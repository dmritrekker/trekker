#include "PTF.h"

PTF::PTF(RandomDoer *_rndmr) {
	init_Frame();
	rndmr          = _rndmr;
    initialized    = false;
}

void PTF::init_Frame() {
	p 			= new float[3];
    
    k1          = 0;
    k2          = 0;
    kT1         = 1;
    kT2         = 0;
    
    F           = new float*[3];
    F[0]        = new float[3];
    F[1]        = new float[3];
    F[2]        = new float[3];

    PP          = new float[8];
    sPP         = new float[8];
    
	likelihood 	= 0.0;
    lastVal     = 0.0;
}

PTF::~PTF() {
	delete[] p;
    
    delete[] F[0];
    delete[] F[1];
    delete[] F[2];
    delete[] F;
    
    delete[] PP;
    delete[] sPP;
    
}

void PTF::initkT(PTF *ptf) {
    kT1 = k1 - ptf->k1; // Sets the kT of the curve
    kT2 = k2 - ptf->k2;
    
    float norm = std::sqrt(kT1*kT1 + kT2*kT2);
    kT1 /= norm;
    kT2 /= norm;
    
    ptf->kT1 = kT1; // Sets the kT of the initial_curve
    ptf->kT2 = kT2;
    
    initialized = true;
    
}

void PTF::swap(PTF *ptf) {

    k1       =  ptf->k1;
    k2       =  ptf->k2;
    k1_cand  =  ptf->k1_cand;
    k2_cand  =  ptf->k2_cand;
    kT1      =  ptf->kT1;
    kT2      =  ptf->kT2;
    
	for (int i=0; i<3; i++) {
        p[i] 	= ptf->p[i];
        for (int j=0; j<3; j++) {
            F[i][j] = ptf->F[i][j];
        }
        
	}
	
	for (int i=0; i<8; i++) {
        sPP[i] = ptf->sPP[i];
    }
	
	likelihood 	 = ptf->likelihood;
    initFirstVal = ptf->initFirstVal;
    lastVal      = ptf->lastVal;
}

void PTF::getARandomFrame() {
	rndmr->getAUnitRandomVector(F[0]);
	rndmr->getAUnitRandomPerpVector(F[2],F[0]);
	cross(F[1],F[2],F[0]);
}

void PTF::getARandomFrame(Coordinate _seed_init_direction) {
	F[0][0] = _seed_init_direction.x;
	F[0][1] = _seed_init_direction.y;
	F[0][2] = _seed_init_direction.z;
	rndmr->getAUnitRandomPerpVector(F[2],F[0]);
	cross(F[1],F[2],F[0]);
}

// To flip PTF parameterized curve
// flip signs of T, K1 and k1
// keep K2 and k2 as they are
void PTF::flip() {
	walk();

	for (int i=0; i<3; i++) {
		F[0][i]  	*= -1;
		F[1][i] 	*= -1;
	}
	k1      *= -1;
    k1_cand *= -1;
    
    kT1 *= -1;
    kT2 *= -1;

	likelihood 	= 0.0;
    lastVal     = initFirstVal;
}


void PTF::print() {
	std::cout << "p:  " << p[0] << " " << p[1] << " " << p[2] << std::endl;
	std::cout << "k:  " << getk() << std::endl;
	std::cout << "k1: " << getk1() << std::endl;
	std::cout << "k2: " << getk2() << std::endl;
	std::cout << "T: "  << F[0][0] << " " <<  F[0][1] << " " <<  F[0][2] << std::endl;
	std::cout << "K1: " << F[1][0] << " " <<  F[1][1] << " " <<  F[1][2] << std::endl;
	std::cout << "K2: " << F[2][0] << " " <<  F[2][1] << " " <<  F[2][2] << std::endl;
	std::cout << "likelihood: " << likelihood << std::endl;
}

