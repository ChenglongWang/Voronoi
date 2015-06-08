#include <mist/mist.h>
#include <mist/filter/distance.h>
#include <vector>
#include <algorithm>
#include <assert.h>
#include "./VesselLabeling/artery.h"
#include "./VesselLabeling/image.h"

extern "C"
{
	__declspec(dllexport) bool Run(const mist::array3<short> *kidneyData, const mist::array3<short> *vesselData, mist::array3<short>* output, double x, double y, double z, int depth);
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
bool Run(const mist::array3<short> *kidneyData, const mist::array3<short> *vesselData, mist::array3<short>* output, double x, double y, double z, int depth)
{
	if (kidneyData->size() != vesselData->size() ||
		kidneyData->size1() != vesselData->size1() ||
		kidneyData->size2() != vesselData->size2())
		return false;

	tmatsu::Point root(x, y, z);
	tmatsu::artery::Tree tree;
	tmatsu::artery::CTImage vessel(*vesselData);
	tmatsu::artery::ThicknessImage centerline;

	vessel.thinning(centerline);
	tree.construct(centerline, root);

	std::vector<mist::vector3<size_t>> endPoints;
	for (auto iter = tree.begin(); iter != tree.end(); ++iter)
	{
		if (!iter.node()->eldest_child)
		{
			endPoints.push_back(iter->value.end_pt);
		}
	}

	for (size_t i = 0; i < endPoints.size(); ++i)
	{
		auto pt = endPoints.at(i);
		(*output)(pt.x, pt.y, pt.z) = i+2;
	}


	mist::euclidean::voronoi_transform(*output);
	
	for (size_t i = 0; i < kidneyData->size(); ++i)
	{
		if ((*kidneyData)[i] == 0)
			(*output)[i] = 0;
	}

	vessel.clear();
	centerline.clear();


	return(true);
}
