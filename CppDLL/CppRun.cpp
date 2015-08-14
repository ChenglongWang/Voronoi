#include <mist/mist.h>
#include <mist/filter/distance.h>
#include <mist/filter/morphology.h>
#include <vector>
#include <algorithm>
#include <assert.h>
#include "./VesselLabeling/artery.h"
#include "./VesselLabeling/image.h"

#include "myVoronoi.h"

extern "C"
{
	__declspec(dllexport) bool Run(const mist::array3<short> *kidneyData, const mist::array3<short> *vesselData, mist::array3<short>* labeledVessel, mist::array3<short>* voronoiData, double x, double y, double z, int kidneyValue, int vesselValue, int depth, bool manualRoot);
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

inline size_t ComputeId(int i, int j, int k, int dim_x, int dim_y, int dim_z)
{
	return (static_cast<size_t>(k)*dim_y + j)*dim_x + i;
}

bool NormalVoronoiMode(const mist::array3<short> *kidneyData, const mist::array3<short> *vesselData, tmatsu::artery::Tree& tree, std::vector<tmatsu::artery::Tree::iterator>& seedNodes)
{
	mist::array3<short> renalAreaContour(*kidneyData); //kidney surface
	mist::dilation(renalAreaContour, 1);
	renalAreaContour -= *kidneyData;
	
	mist::array3<unsigned char> labelingData(kidneyData->size1(), kidneyData->size2(), kidneyData->size3(),
									 kidneyData->reso1(), kidneyData->reso2(), kidneyData->reso3());
	mist::array3<short> labeledData(kidneyData->size1(), kidneyData->size2(), kidneyData->size3(),
									kidneyData->reso1(), kidneyData->reso2(), kidneyData->reso3());
	labelingData.fill(0);
	labeledData.fill(0);

	for (size_t i = 0; i < kidneyData->size(); ++i)
	{
		if (renalAreaContour[i] != 0 && (*vesselData)[i] != 0)
			labelingData[i] = 1;
	}

	auto labelNum = mist::labeling26(labelingData, labeledData);

	std::vector<size_t> labeledPoints;
	std::vector<std::vector<size_t>> labeledPointsContainer;
	for (int i = 1; i < labelNum+1; ++i)
	{
		for (size_t index = 0; index < labeledData.size(); ++index)
		{
			if (labeledData[index] == i)
			{
				labeledPoints.push_back(index);
			}
		}
		labeledPointsContainer.push_back(labeledPoints);
	}

	//Loop all the tree node, extract the centerline with id and tree_iter labeled.
	std::map<size_t, tmatsu::artery::Tree::iterator> centerLineToTreeMap;
	for (auto iter = tree.begin(); iter != tree.end(); ++iter)
	{
		auto pt = iter->value.start_pt;
		auto path = iter->value.path;
		for_each(path.begin(), path.end(), [&](size_t offset){
			pt += tmatsu::TOPOLOGICAL(neighborhood)[offset];
			auto id = ComputeId(pt.x, pt.y, pt.z, kidneyData->size1(), kidneyData->size2(), kidneyData->size3());
			centerLineToTreeMap.insert(std::make_pair(id, iter));
		});
	}

	std::map<size_t, tmatsu::artery::Tree::iterator> treeCrossNode;
	for (int i = 0; i < labeledPointsContainer.size(); ++i)
	{
		auto labeledPts = labeledPointsContainer[i];
		for_each(labeledPts.begin(), labeledPts.end(), [&](size_t id){
			if (centerLineToTreeMap.find(id) != centerLineToTreeMap.end())
			{
				treeCrossNode.insert(std::make_pair(centerLineToTreeMap[id]->id, centerLineToTreeMap[id]));
			}
		});
	}

	for (int i = 0; i < treeCrossNode.size(); ++i)
	{
		if (treeCrossNode[i].node() != NULL)
			seedNodes.push_back(treeCrossNode[i]);
	}
	return true;
}

void LabelVessel(std::vector<tmatsu::artery::Tree::iterator> &seedNodes, tmatsu::artery::Tree &tree, int vesselValue)
{
	int labels(vesselValue);

	//Delete the redundant nodes
	for (auto iter = seedNodes.begin(); iter != seedNodes.end();)
	{
		if (iter->node() == NULL)
		{
			iter = seedNodes.erase(iter);
		}
		else
		{
			bool flag = false;
			for (auto parentIter = seedNodes.begin(); parentIter != seedNodes.end(); ++parentIter)
			{
				if (tree.is_ancestor(*parentIter, *iter))
				{
					iter = seedNodes.erase(iter);
					flag = true;
					break;
				}
			}
			if (!flag)
				++iter;
		}
	}

	//Label vessels
	for_each(seedNodes.begin(), seedNodes.end(), [&](tmatsu::artery::Tree::iterator node){
		if (node.node() != NULL)
		{
			for (auto iter = tree.child_begin(node); iter != tree.child_end(node); ++iter)
			{
				iter->name = tmatsu::artery::Name(labels);
			}
			node->name = tmatsu::artery::Name(labels);

			labels++;
		}
	});
}

void LabelOrgan(const mist::array3<short>& labeledVessels, mist::array3<short>& voronoiData)
{
	for (size_t i = 0; i < voronoiData.size(); ++i)
	{
		if (voronoiData[i] != 0 && labeledVessels[i] != 0)
		{
			voronoiData[i] = labeledVessels[i];
		}
	}
}

/*Voronoi transform need renal part mask input, and vessel data. 
{x,y,z} is a point closet to the root point of the vessel. Depth is the search depth 
of the vessel tree*/
bool Run(const mist::array3<short> *kidneyData, const mist::array3<short> *vesselData, mist::array3<short>* labeledVessel, mist::array3<short>* out, double x, double y, double z, int kidneyValue, int vesselValue, int depth, bool mannulRoot)
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

	tmatsu::artery::Tree tree;
	mist::array3<short> vessel_closed(*vesselData);
#ifndef _DEBUG
	mist::dilation(vessel_closed, 1);
	mist::closing(vessel_closed, 4);
#endif
	

	tmatsu::artery::CTImage vessel(vessel_closed);
	tmatsu::artery::ThicknessImage centerline;

	vessel.thinning(centerline);
	if (mannulRoot)
		tree.construct(centerline, tmatsu::Point(x, y, z));
	else
		tree.construct(centerline);

	std::vector<tmatsu::artery::Tree::iterator> seedNodes;
	if (depth == 0)
	{
		if (!NormalVoronoiMode(kidneyData, vesselData, tree, seedNodes))
			return false;
	}
	else
	{
		for (auto iter = tree.begin(); iter != tree.end(); ++iter)
		{
			if (tree.depth(iter) == depth)
			{
				seedNodes.push_back(iter);
			}
		}
	}

	LabelVessel(seedNodes, tree, vesselValue);

	tmatsu::artery::LabelImage labelVesselImage;
	tree.restore(*vesselData, labelVesselImage);

	*labeledVessel = labelVesselImage;

//	LabelOrgan(*labeledVessel, voronoiData);

//	mist::euclidean::voronoi_transform(*voronoiData);
	myvoronoi(*kidneyData, *labeledVessel, *out, seedNodes.size(), kidneyValue, vesselValue);

	vessel.clear();
	centerline.clear();
	tree.clear();

	return(true);
}
