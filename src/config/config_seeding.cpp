#include "config_seeding.h"

using namespace GENERAL;

namespace SEED {

ROI_Image* img_SEED 		= new ROI_Image;
std::vector<size_t>   		seed_indices;
std::vector<Coordinate>     seed_coordinates;
std::vector<Coordinate>     seed_init_directions;
std::string seed_coordinate_fname;
std::string seed_init_directions_fname;
SeedingMode seedingMode    	= SEED_NOTSET;
int count 					= NOTSET;
int countPerVoxel 			= NOTSET;
int maxTrialsPerSeed 		= NOTSET;


void cleanConfigSeeding() {
	delete img_SEED;
}

void setDefaultParametersWhenNecessary() {

	if (seedingMode!=SEED_IMAGE) {
		if (count!=NOTSET) {
			if (GENERAL::verboseLevel!=QUITE) std::cout << "-seed_count is ignored since -seed_coordinates is defined, ";
			if (GENERAL::verboseLevel!=QUITE) std::cout << "count is forced to be:" << seed_coordinates.size() << std::endl;
		}
		if (countPerVoxel!=NOTSET) {
			if (GENERAL::verboseLevel!=QUITE) std::cout << "-seed_countPerVoxel is ignored since -seed_coordinates is defined, ";
			if (GENERAL::verboseLevel!=QUITE) std::cout << "count is forced to be:" << seed_coordinates.size() << std::endl;
		}
		count 			= seed_coordinates.size();
		if (count>MAXNUMBEROFSEEDS){
			if (GENERAL::verboseLevel!=QUITE) std::cout << "Maximum number of seeds cannot exceed 1e9" << std::endl;
			exit(EXIT_FAILURE);
		}
		countPerVoxel 	= NOTSET;
	} else {
		if ((count!=NOTSET) && (countPerVoxel!=NOTSET)) {
			countPerVoxel = NOTSET;
			if (GENERAL::verboseLevel!=QUITE) std::cout << "-countPerVoxel is ignored since -seed_count is defined. " << std::endl;
		}
		if ((count==NOTSET) && (countPerVoxel==NOTSET)) {
			count = MAXNUMBEROFSEEDS;
		}
	}

	if (maxTrialsPerSeed==NOTSET)
		maxTrialsPerSeed=MAXTRIALSPERSEEDS;

}

void readSeedImage() {
	if (seedingMode!=SEED_IMAGE)
		return;

	if (GENERAL::verboseLevel!=QUITE) std::cout << "Reading seed image                 : " << img_SEED->getFilePath() << std::endl;

	if (!img_SEED->readImage())
		exit(EXIT_FAILURE);

	for(size_t i=0; i<img_SEED->getNim()->nvox; i++)
		if (img_SEED->getVal(i))
			seed_indices.push_back(i);

	if (seed_indices.empty()) {
		std::cout << "Seed image ";

		int label;
		if (img_SEED->getLabel(label))
			std::cout << "(using label " << label << ") ";

		std::cout << "is empty"  << std::endl;
		exit(EXIT_FAILURE);
	}


	if (countPerVoxel!=NOTSET) {
		count = seed_indices.size()*countPerVoxel;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "   -> setting count to be " << count << std::endl;
		if (count>MAXNUMBEROFSEEDS){
			std::cout << "Maximum number of seeds cannot exceed 1e9" << std::endl;
			std::cout << " For this seed image, maximum countPerVoxel cannot exceed " << floor(MAXNUMBEROFSEEDS/seed_indices.size()) << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	// If seed image is defined together with a certain count then use a dilated version of the seed image to account for partial volumes for better sampling
	if (countPerVoxel==NOTSET) {

		bool* dilated_img_SEED = new bool[img_SEED->getNim()->nvox];
		memset(dilated_img_SEED,false,(img_SEED->getNim()->nvox)*sizeof(bool));

		for (std::vector<size_t>::iterator it = seed_indices.begin(); it != seed_indices.end(); ++it)
			dilated_img_SEED[(*it)] = true;

		std::vector<size_t> dilated_seed_indices;

		for (std::vector<size_t>::iterator it = seed_indices.begin(); it != seed_indices.end(); ++it) {
			int i,j,k;
			img_SEED->ind2sub((*it),i,j,k);

			for (int x=-1; x<2; x++)
				for (int y=-1; y<2; y++)
					for (int z=-1; z<2; z++)
						if ( !((x==0)&&(y==0)&&(z==0)) && (img_SEED->checkImageBounds(i+x,j+y,k+z)!=0) ) {

							size_t index = img_SEED->sub2ind(i+x,j+y,k+z);

							if ((img_SEED->getVal(index)==0) && (dilated_img_SEED[index]==false)) {
								dilated_img_SEED[index] = true;
								dilated_seed_indices.push_back(index);
							}

						}

		}
		delete[] dilated_img_SEED;

		seed_indices.insert( seed_indices.end(), dilated_seed_indices.begin(), dilated_seed_indices.end() );

	}

}

void print() {
	std::cout << std::endl;
	std::cout << "SEEDING OPTIONS"<< std::endl;

	if ((count!=NOTSET) && (count!=MAXNUMBEROFSEEDS)) 			std::cout << "count                : " << count << std::endl;
	if ((seedingMode==SEED_IMAGE) && (countPerVoxel!=NOTSET)) 	std::cout << "countPerVoxel        : " << countPerVoxel << std::endl;

	std::cout << "maxTrialsPerSeed     : " << maxTrialsPerSeed << std::endl;

	std::cout << "seedingMode          : ";
	switch (seedingMode) {
	case SEED_IMAGE:                		std::cout << "seed_image"              			<< std::endl; break;
	case SEED_COORDINATES:              	std::cout << "seed_coordinates"              	<< std::endl; break;
	case SEED_COORDINATES_WITH_DIRECTIONS: 	std::cout << "seed_coordinates_with_directions" << std::endl; break;
	default: break;
	}

	if (seedingMode==SEED_IMAGE) {
		if (GENERAL::verboseLevel>ON) std::cout << std::endl << "-----------------" << std::endl;
		std::cout << "seed image           : ";

		int label;
		if (img_SEED->getLabel(label))
			std::cout << "(using label " << label << ") ";

		std::cout << img_SEED->getFilePath()  << std::endl;

		if (GENERAL::verboseLevel>ON) img_SEED->printInfo();
		if (GENERAL::verboseLevel>ON) std::cout << "-----------------" << std::endl << std::endl;
	}
}

}
