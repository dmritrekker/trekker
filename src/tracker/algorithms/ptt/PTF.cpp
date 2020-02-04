#include "PTF.h"

#define SMALL 0.0001

PTF::PTF(RandomDoer *_rndmr) {
	init_Frame();
	rndmr          = _rndmr;
    initialized    = false;
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
        F[0][i] =  T[i];
        F[1][i] = N1[i];
        F[2][i] = N2[i];
    }
}

void PTF::initkT(PTF *ptf) {
    kT1 = ptf->k1 - k1;
    kT2 = ptf->k2 - k2;
    
    float norm = std::sqrt(kT1*kT1 + kT2*kT2);
    
    kT1 /= norm;
    kT2 /= norm;
    
    initialized = true;
}

void PTF::swap(PTF *ptf) {

    k1  =  ptf->k1;
    k2  =  ptf->k2;
    kT1 =  ptf->kT1;
    kT2 =  ptf->kT2;
    
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

	k1  *= -1;
    kT1 *= -1;
    kT2 *= -1;

	likelihood 	=  0.0;
	prior 		=  1.0;
	posterior 	= -1.0; // This is used to check if the curve is swapped with a candidate curve
}


void PTF::walk() {

    this->prepStepPropagator();
    
	for (int i=0; i<3; i++) {
		p[i]  += PP[0]*F[0][i] +  PP[1]*F[1][i]  +  PP[2]*F[2][i];
		T[i]   = PP[3]*F[0][i] +  PP[4]*F[1][i]  +  PP[5]*F[2][i];
		N2[i]  = PP[6]*F[0][i] +  PP[7]*F[1][i]  +  PP[8]*F[2][i];
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

void PTF::prepCandStepPropagator() {
    
    switch (TRACKER::algorithm) {
        
        case PTT_C1: {
            prepCandStepPropagator_C1();
            break;
        }
            
        case PTT_C2: {
            
            if ((std::fabs(k1-k1_cand)<SMALL) && (std::fabs(k2-k2_cand)<SMALL)) {
                prepCandStepPropagator_C1();
            } else {
                prepCandStepPropagator_C2();
            }
            
            break;
        }
            
        case PTT_C3: {
            
            if (initialized==false) {
                prepCandStepPropagator_C1();
                kT1_cand = kT1;
                kT2_cand = kT2;
                return;
            }
            
            float q1 = k1_cand - k1;
            float q2 = k2_cand - k2;
            
            float nq = std::sqrt(q1*q1+q2*q2);
            
            if (nq<SMALL) {
                prepCandStepPropagator_C1();
                kT1_cand = kT1;
                kT2_cand = kT2;
                return;
            }
            
            float cosTheta= (q1*kT1+q2*kT2)/nq;
            
            if (abs(abs(cosTheta)-1)<SMALL) {                
                prepCandStepPropagator_C2();
                kT1_cand = kT1;
                kT2_cand = kT2;
                return;
            }
            
            float theta   = std::acos(cosTheta);
            float sinTheta= std::sin(theta);
            float r       = nq/(2*sinTheta);
            float cqt     = kT2*q1-kT1*q2;
            
            float dirC1, dirC2;
            if (cqt>0) {
                theta   = -theta;
                dirC1   =  kT2;
                dirC2   = -kT1;
            } else {
                dirC1   = -kT2;
                dirC2   =  kT1;
            }
            
            float xc = dirC1*r+k1;
            float yc = dirC2*r+k2;

            float alpha = std::atan2(k2-yc,k1-xc);
            float sign  = (theta > 0) ? 1 : -1;
            kT1_cand    = std::cos(alpha + 2*theta/TRACKER::stepSize + sign*M_PI_2);
            kT2_cand    = std::sin(alpha + 2*theta/TRACKER::stepSize + sign*M_PI_2);
            
            prepCandStepPropagator_C3(r,xc,yc,alpha,theta);
            
            
            break;
        }
            
        default: { break; }
    }
    
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
