#include <mist/mist.h>
#include <mist/filter/region_growing.h>
#include <vector>
#include <algorithm>
#include <assert.h>

extern "C"
{
  __declspec( dllexport ) bool Run( const mist::array3<short> *originImage, mist::array3<unsigned char> *maskImage, int radius );
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

bool Run( const mist::array3<short> *originImage, mist::array3<unsigned char> *maskImage, int radius )
{  
	int id(0);
	std::vector<position_type> point_list;

	auto dim_x = maskImage->size1();
	auto dim_y = maskImage->size2();
	auto dim_z = maskImage->size3();

    std::for_each(maskImage->begin(), maskImage->end(), [&](unsigned char& pixel){
		if (pixel != 0)
			point_list.push_back(ComputeIJK(id, dim_x, dim_y, dim_z));
		id++;
    });

  return( true );
}
