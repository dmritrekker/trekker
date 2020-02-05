#include "PTF.h"

#define SMALL 0.0001

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
	prior 		= 1.0;
	posterior 	=-1.0; // This is used to check if the curve is swapped with a candidate curve
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
    kT1 = ptf->k1 - k1; // Sets the kT of the curve
    kT2 = ptf->k2 - k2;
    
    float norm = std::sqrt(kT1*kT1 + kT2*kT2);
    kT1 /= norm;
    kT2 /= norm;
    
    ptf->kT1 = kT1; // Sets the kT of the initial_curve
    ptf->kT2 = kT2;
    
    initialized = true;
}

void PTF::swap(PTF *ptf) {

    k1  =  ptf->k1;
    k2  =  ptf->k2;
    kT1 =  ptf->kT1;
    kT2 =  ptf->kT2;
    
	for (int i=0; i<3; i++) {
        p[i] 	= ptf->p[i];
        for (int j=0; j<3; j++) {
            F[i][j] = ptf->F[i][j];
        }
        
	}
	
	for (int i=0; i<8; i++) {
        sPP[i] = ptf->sPP[i];
    }
	
	likelihood 	= ptf->likelihood;
	prior 		= ptf->prior;
	posterior 	= ptf->posterior;

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
	k1  *= -1;
    
    kT1 *= -1;
    kT2 *= -1;

	likelihood 	=  0.0;
	prior 		=  1.0;
	posterior 	= -1.0; // This is used to check if the curve is swapped with a candidate curve
}


void PTF::walk() {
    
    prepCandStepPropagator();
    
    float T[3];
    
	for (int i=0; i<3; i++) {
		p[i]   += sPP[0]*F[0][i] +  sPP[1]*F[1][i]  +  sPP[2]*F[2][i];
		T[i]    = sPP[3]*F[0][i] +  sPP[4]*F[1][i]  +  sPP[5]*F[2][i];
		F[2][i] = sPP[6]*F[0][i] +  sPP[7]*F[1][i]  +  sPP[8]*F[2][i];
	}

	normalize(T);
	cross(F[1],F[2],T);
	normalize(F[1]);
	cross(F[2],T,F[1]);
    
    F[0][0]=T[0];
    F[0][1]=T[1];
    F[0][2]=T[2];
    
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

void PTF::prepProbePropagator() {
    
    switch (TRACKER::algorithm) {
        
        case PTT_C1: {
            if (probeStepCount==0) {
                prepProbePropagator_C1();
            }
            break;
        }
            
        case PTT_C2: {
            
            if ((std::fabs(k1-k1_cand)<SMALL) && (std::fabs(k2-k2_cand)<SMALL)) {
                if (probeStepCount==0) {
                    prepProbePropagator_C1();
                }
            } else {
                if (probeStepCount==0) {
                    probe_k1 = k1;
                    probe_k2 = k2;
                }
                prepProbePropagator_C2();
            }
            
            break;
        }
            
        case PTT_C3: {

        }
            
        default: { break; }
    }
    
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
	std::cout << "prior: "      << prior      << std::endl;
	std::cout << "posterior: "  << posterior  << std::endl << std::endl;
}
