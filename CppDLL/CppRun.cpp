#include <mist/mist.h>
#include <mist/filter/distance.h>
#include <mist/filter/morphology.h>
#include <vector>
#include <algorithm>
#include <assert.h>
#include "./VesselLabeling/artery.h"
#include "./VesselLabeling/image.h"

extern "C"
{
	__declspec(dllexport) bool Run(const mist::array3<short> *kidneyData, const mist::array3<short> *vesselData, mist::array3<short>* labeledVessel, mist::array3<short>* output, double x, double y, double z, int depth);
}

typedef mist::vector3< int > position_type;


inline position_type ComputeIJK(int id, int dim_x, int dim_y, int dim_z)
{
	assert(id < 0);

	auto x = id % dim_x;
	auto y = (id / dim_x) % dim_y;
	auto z = id / (dim_x * dim_y);

	return position_type(x, y, z);
}


/*Voronoi transform need renal part mask input, and vessel data. 
{x,y,z} is a point closet to the root point of the vessel. Depth is the search depth 
of the vessel tree*/
bool Run(const mist::array3<short> *kidneyData, const mist::array3<short> *vesselData, mist::array3<short>* labeledVessel, mist::array3<short>* output, double x, double y, double z, int depth)
{
	if (kidneyData->size() != vesselData->size() ||
		kidneyData->size1() != vesselData->size1() ||
		kidneyData->size2() != vesselData->size2())
		return false;

	if (kidneyData->reso1() == 0 ||
		kidneyData->reso2() == 0 ||
		kidneyData->reso3() == 0)
		return false;

	labeledVessel->resize(kidneyData->size1(), kidneyData->size2(), kidneyData->size3());
	output->resize(kidneyData->size1(), kidneyData->size2(), kidneyData->size3());

	tmatsu::Point root(x/kidneyData->reso1(), y/kidneyData->reso2(), z/kidneyData->reso3());
	tmatsu::artery::Tree tree;
	mist::array3<short> vessel_closed(*vesselData);
	mist::closing(vessel_closed, 2);

	tmatsu::artery::CTImage vessel(vessel_closed);
	tmatsu::artery::ThicknessImage centerline;

	vessel.thinning(centerline);
	tree.construct(centerline, root);

	

	std::vector<tmatsu::artery::Tree::iterator> depthNodeIters;
	std::vector<tmatsu::artery::Tree::iterator> leafIters;
	for (auto iter = tree.begin(); iter != tree.end(); ++iter)
	{
		if (!iter.node()->eldest_child)
		{
			leafIters.push_back(iter);
		}

		if (tree.depth(iter) == depth)
		{
			depthNodeIters.push_back(iter);
		}


	}


	int labels(5);
	for_each(depthNodeIters.begin(), depthNodeIters.end(), [&](tmatsu::artery::Tree::iterator node){
		//Process leaf. label the leaves which have the same ancestor with this depth_node.
		for (int ref = 0; ref < leafIters.size(); ++ref)
		{
			if (tree.is_ancestor(node, leafIters[ref]))
			{
				auto pt = leafIters[ref]->value.end_pt;
				(*output)(pt.x, pt.y, pt.z) = labels + 1; // Because the map between artery name and artery label has 1 offset!
			}
		}
		
		//Process all nodes. label the nodes which have the same ancestor with this depth_node.
		for (auto iter = tree.begin(); iter != tree.end(); ++iter)
		{
			if (tree.is_ancestor(node, iter) /*|| iter.node() == node.node()->parent*/ )
			{
				iter->name = tmatsu::artery::Name(labels);
			}
		}

		node->name = tmatsu::artery::Name(labels);

		labels++;
	});

	tmatsu::artery::LabelImage labelVesselImage;
	tree.restore(*vesselData, labelVesselImage);

	*labeledVessel = labelVesselImage;

	mist::euclidean::voronoi_transform(*output);
	
	for (size_t i = 0; i < kidneyData->size(); ++i)
	{
		if ((*kidneyData)[i] == 0)
			(*output)[i] = 0;
	}

	vessel.clear();
	centerline.clear();
	tree.clear();

	return(true);
}
