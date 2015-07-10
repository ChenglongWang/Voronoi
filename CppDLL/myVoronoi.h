#ifndef __KIDNEY_DIVIDE_VORONOI_HEADER__
#define __KIDNEY_DIVIDE_VORONOI_HEADER__

#include <algorithm>
#include <fstream>


// �����l���i�[�����z�񒆂���ŏ��l���擾
// �z��̓��x���l���ɕ���ł���̂ŁA���̏��Ԃ�Ԃ�
int calc_min(mist::array <double > value)
{
	int ret = -1;

	// �\�[�g�O�ɏ��Ԃ�ێ����邽�ߔz����R�s�[
	mist::array < double > tmp = value;

	// ���������Ƀ\�[�g
	std::sort(value.begin(), value.end(), std::less<double>());

	// �ŏ��l���擾
	double min = value.at(0);

	// �ŏ��l�̔ԍ��𒲂ׂ�
	for (int i = 0; i < value.size(); i++)
	{
		if (min == tmp.at(i))
		{
			ret = i;
			break;
		}
	}

	// �ŏ��l�̔ԍ���Ԃ�
	return ret + 1;
}

int min3(double v1, double v2, double v3)
{
	int ret = -1;
	if (v1 < v2)
	{
		if (v1 < v3)
		{
			ret = 1;
		}
		else
		{
			ret = 3;
		}
	}
	else
	{
		if (v2 < v3)
		{
			ret = 2;
		}
		else
		{
			ret = 3;
		}
	}


	return ret;
}



int max3(double v1, double v2, double v3)
{
	int ret = -1;
	if (v1 > v2)
	{
		if (v1 > v3)
		{
			ret = 1;
		}
		else
		{
			ret = 3;
		}
	}
	else
	{
		if (v2 > v3)
		{
			ret = 2;
		}
		else
		{
			ret = 3;
		}
	}
	return ret;
}

// ���͓_���m�̋����l����
// ���́F���W�A�𑜓x�A��_���W���X�g�imm)
double distance(int x, int y, int z, double xr, double yr, double zr, mist::vector3 < double > gp)
{

	double px = x * xr - gp.x;
	double py = y * yr - gp.y;
	double pz = z * zr - gp.z;
	double dist = px * px + py * py + pz * pz;


	return (sqrt(dist));

}

// ���͍��W�i�𑜓x�t���j�ƕ�_���X�g�̊e�_�̒��ōŏ��̋����l��Ԃ�
double point_min(int x, int y, int z, double xr, double yr, double zr, std::vector < mist::vector3 < double > > gp)
{
	std::vector < double > dist2;
	// �܂��A���x�����Ƃɍŏ��̋���
	for (int i = 0; i < gp.size(); i++)
	{
		// �����l�𑪒肵�Ēǉ�
		// �����l�͕������Ƃ��Ă���
		dist2.push_back(distance(x, y, z, xr, yr, zr, gp.at(i)));
	}

	// �����l�����������ɕ��ёւ�
//	std::sort(dist2.begin(), dist2.end(), std::less<double>());
	auto minElem = std::min_element(dist2.begin(), dist2.end());

	return *minElem;
}


// �̐ϊ�������
void calc_volume(const mist::array3 < short > &img, const std::vector<short>& labels, std::string csvfile)
{
	int num = labels.size();
	mist::array < int > volume(num);

	int all_volume = 0;

	double reso = img.reso1() * img.reso2() * img.reso3();

	for (int i = 0; i < img.size(); ++i)
	{
		if (img[i] > 0)
		{
			++all_volume;

			for (int index = 0; index < num; ++index)
			{
				if (img[i] == labels[index])
				{
					++volume.at(index);
				}
			}
		}
	}

	std::ofstream ofs(csvfile, std::ios::out);

	ofs << "Label,Volume,Ratio," << std::endl;

	ofs << "all," << (double)all_volume * reso << ",100," << std::endl;
	for (int i = 0; i < num; ++i)
	{
		ofs << labels[i] << "," << (double) volume.at(i) * reso << "," << ((double)volume.at(i) / (double) all_volume) * 100.0  << "," << std::endl;

	}

	ofs.close();

}



// ����{���m�C����
// ���[�N���b�h�����̂�
// �Ώۗ̈�:128�A���x���F1,2�A3�A4,....
// �e���x���̒l�̘A���������_�Ƃ��ă{���m�C����
// img�F���͉摜�Aout�F�o�͉摜�Anum�F���x����
bool myvoronoi(const mist::array3 < short > & img, const mist::array3<short>& vessels, mist::array3 < short > &out, int num, int organValue)
{
	if (img.size1() != vessels.size1() ||
		img.size2() != vessels.size2() ||
		img.size3() != vessels.size3())
		return false;

	int area = organValue;
	
	double xr = img.reso1();
	double yr = img.reso2();
	double zr = img.reso3();

	//Caluate voi size
	size_t voi_x0(INT_MAX), voi_x1(0), voi_y0(INT_MAX), voi_y1(0), voi_z0(INT_MAX), voi_z1(0);
	for (size_t k = 0; k < img.depth(); ++k)
	{
		for (size_t j = 0; j < img.height(); ++j)
		{
			for (size_t i = 0; i < img.width(); ++i)
			{
				if (img(i, j, k) == area)
				{
					voi_x0 = std::min(i, voi_x0);
					voi_x1 = std::max(i, voi_x1);
					voi_y0 = std::min(j, voi_y0);
					voi_y1 = std::max(j, voi_y1);
					voi_z0 = std::min(k, voi_z0);
					voi_z1 = std::max(k, voi_z1);
				}
			}
		}
	}

	//Make margin.
	voi_x0 = (voi_x0 - 2) > 0 ? voi_x0 - 2 : 0;
	voi_x1 = (voi_x1 + 2) < (img.size1() - 1) ? voi_x1 + 2 : (img.size1() - 1);
	voi_y0 = (voi_y0 - 2) > 0 ? voi_y0 - 2 : 0;
	voi_y1 = (voi_y1 + 2) < (img.size2() - 1) ? voi_y1 + 2 : (img.size2() - 1);
	voi_z0 = (voi_z0 - 2) > 0 ? voi_z0 - 2 : 0;
	voi_z1 = (voi_z1 + 2) < (img.size3() - 1) ? voi_z1 + 2 : (img.size3() - 1);

	short value(VESSEL_LABEL);
	std::vector<short> values;
	std::vector< mist::vector3<double> > pts;
	std::map<short, std::vector< mist::vector3<double> >> gp;

	while (gp.size() < num)
	{
		pts.clear();
		for (size_t k = voi_z0; k < voi_z1; ++k)
		{
			for (size_t j = voi_y0; j < voi_y1; ++j)
			{
				for (size_t i = voi_x0; i < voi_x1; ++i)
				{
					if (img(i, j, k) == area)
					{
						if (vessels(i, j, k) == value)
						{
							pts.push_back(mist::vector3<double>(i * xr, j * yr, k * zr));
						}
					}
				}
			}
		}
		
		if (!pts.empty())
		{
			values.push_back(value);
			gp.insert(std::make_pair(value, pts));
		}
		++value;
	}

	// �{���m�C��������
	for (size_t k = voi_z0; k < voi_z1; ++k)
	{
		for (size_t j = voi_y0; j < voi_y1; ++j)
		{
			for (size_t i = voi_x0; i < voi_x1; ++i)
			{
				// �t���̈��̉�f�݂̂ŏ���
				if (img.at(i, j, k) == area)
				{
					// �e���x���̍ŏ��������i�[�p
					mist::array < double > dist(num);

					// ���x���l���Ƃɋ����l�𑪒�
					for (int n = 0; n < values.size(); ++n)
					{
						dist.at(n) = point_min((int)i, (int)j, (int)k, xr, yr, zr, gp.at(values[n]));
					}

					auto minIter = std::min_element(dist.begin(), dist.end());
					auto min = std::distance(dist.begin(), minIter);

					// �擾�������x���l���o�͉摜�֊i�[
					out(i, j, k) = values[min];
				}
			}
		}
	}

	calc_volume(out, values, "./volume.csv");

	return true;
}

#endif // __KIDNEY_DIVIDE_VORONOI_HEADER__