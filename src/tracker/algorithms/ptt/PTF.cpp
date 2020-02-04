#include "PTF.h"

PTF::PTF(RandomDoer *_rndmr) {
	init_Frame();
	rndmr = _rndmr;
}

void PTF::init_Frame() {
	p 			= new float[3];
	T 			= new float[3];
	N1			= new float[3];
	N2			= new float[3];
    
    k1          = 0;
    k2          = 0;
    kT1         = 1;
    kT2         = 0;
    
    F           = new float*[3];
    F[0]        = new float[3];
    F[1]        = new float[3];
    F[2]        = new float[3];

    PP          = new float[8];
    memset(PP,0,8*sizeof(float));
    
	likelihood 	= 0.0;
	prior 		= 1.0;
	posterior 	=-1.0; // This is used to check if the curve is swapped with a candidate curve
}

PTF::~PTF() {
	delete[] p;
	delete[] T;
	delete[] N1;
	delete[] N2;
    
    delete[] F[0];
    delete[] F[1];
    delete[] F[2];
    delete[] F;
    
    delete[] PP;
}

void PTF::updateF() {
    for (int i=0; i<3; i++) {
        F[i][0] =  T[i];
        F[i][1] = N1[i];
        F[i][2] = N2[i];
    }
}

void PTF::swap(PTF *ptf) {

    k1  =  ptf->k1;
    k2  =  ptf->k2;
    kT1 = -ptf->kT1;
    kT2 = -ptf->kT2;
    
	for (int i=0; i<3; i++) {
		 p[i] 	= ptf->p[i];
		 T[i] 	= ptf->T[i];
		N1[i] 	= ptf->N1[i];
		N2[i] 	= ptf->N2[i];
        
        for (int j=0; j<3; j++) {
            F[i][j] = ptf->F[i][j];
        }
        
	}
	
	likelihood 	= ptf->likelihood;
	prior 		= ptf->prior;
	posterior 	= ptf->posterior;

}

void PTF::getARandomFrame() {
	rndmr->getAUnitRandomVector(T);
	rndmr->getAUnitRandomPerpVector(N2,T);
	cross(N1,N2,T);
    updateF();
}

void PTF::getARandomFrame(Coordinate _seed_init_direction) {
	T[0] = _seed_init_direction.x;
	T[1] = _seed_init_direction.y;
	T[2] = _seed_init_direction.z;
	rndmr->getAUnitRandomPerpVector(N2,T);
	cross(N1,N2,T);
    updateF();
}

// To flip PTF parameterized curve
// flip signs of T, N1 and k1
// keep N2 and k2 as they are
void PTF::flip() {
	this->walk();

	for (int i=0; i<3; i++) {
		T[i]  	*= -1;
		N1[i] 	*= -1;
	}
	updateF();

	k1 *= -1;

	likelihood 	=  0.0;
	prior 		=  1.0;
	posterior 	= -1.0; // This is used to check if the curve is swapped with a candidate curve
}


void PTF::walk() {

    this->prepStepPropagator();
    
	for (int i=0; i<3; i++) {
		p[i]  += PP[0]*F[i][0] +  PP[1]*F[i][1]  +  PP[2]*F[i][2];
		T[i]   = PP[3]*F[i][0] +  PP[4]*F[i][1]  +  PP[5]*F[i][2];
		N2[i]  = PP[6]*F[i][0] +  PP[7]*F[i][1]  +  PP[8]*F[i][2];
	}

	normalize(T);
	cross(N1,N2,T);
	normalize(N1);
	cross(N2,T,N1);
    
    updateF();

	likelihood 	=   0.0;
	prior 		=   1.0;
	posterior 	=  -1.0; // This is used to check if the curve is swapped with a candidate curve

}



void PTF::print() {
	std::cout << "p:  " << p[0] << " " << p[1] << " " << p[2] << std::endl;
	std::cout << "k:  " << getk() << std::endl;
	std::cout << "k1: " << getk1() << std::endl;
	std::cout << "k2: " << getk2() << std::endl;
	std::cout << "T: "  <<  T[0] << " " <<  T[1] << " " <<  T[2] << std::endl;
	std::cout << "N1: " << N1[0] << " " << N1[1] << " " << N1[2] << std::endl;
	std::cout << "N2: " << N2[0] << " " << N2[1] << " " << N2[2] << std::endl;
	std::cout << "likelihood: " << likelihood << std::endl;
	std::cout << "prior: " << prior << std::endl;
	std::cout << "posterior: " << posterior << std::endl << std::endl;
}
