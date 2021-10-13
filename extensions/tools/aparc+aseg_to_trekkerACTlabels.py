#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Mar  6 20:21:53 2020

@author: Baran Aydogan
"""
import sys
import nibabel as nib
import numpy as np

DESCRIPTION = """
    Converts Freesurfer aparc+aseg image to do anatomically constraint whole brain tractography with Trekker
    
    Example:
        python aparc+aseg_to_trekkerACTlabels.py aparc+aseg.nii.gz trekkerACTlabels.nii.gz \
"""

def main():
    
    if (len(sys.argv)!=3):
        print('Wrong input. Example: python aparc+aseg_to_trekkerACTlabels.py aparc+aseg.nii.gz trekkerACTlabels.nii.gz');
    else:    
        nifti = nib.load(sys.argv[1]);
        data  = np.asarray(nifti.dataobj)
        data  = data.astype(int)
        out   = np.zeros_like(data)-1
        
        # Label=2 for gray matter
        out  += 3*(data>0)    # The whole brain is seed
        
        # Label=1 for white matter and alike, i.e. pathway=discard_if_ends inside
        out  -= (data==2);   # Left-Cerebral-White-Matter
        out  -= (data==7);   # Left-Cerebellum-White-Matter
        out  -= (data==28);  # Left-VentralDC
        out  -= (data==155); # Left-IntCapsule-Ant
        out  -= (data==157); # Left-IntCapsule-Pos
        out  -= (data==41);  # Right-Cerebral-White-Matter
        out  -= (data==46);  # Right-Cerebellum-White-Matter
        out  -= (data==60);  # Right-VentralDC
        out  -= (data==156); # Right-IntCapsule-Ant
        out  -= (data==158); # Right-IntCapsule-Pos
        out  -= (data==85);  # Optic-Chiasm
        out  -= (data==86);  # Corpus_Callosum
        out  -= (data==251); # CC_Posterior
        out  -= (data==252); # CC_Mid_Posterior
        out  -= (data==253); # CC_Central
        out  -= (data==254); # CC_Mid_Anterior
        out  -= (data==255); # CC_Anterior
        
        # Label=0 for csf, i.e. pathway=discard_if_enters
        out  -= 2*(data==4);    # Left-Lateral-Ventricle
        out  -= 2*(data==5);    # Left-Inf-Lat-Vent
        out  -= 2*(data==30);   # Left-vessel
        out  -= 2*(data==31);   # Left-choroid-plexus
        out  -= 2*(data==43);   # Right-Lateral-Ventricle
        out  -= 2*(data==44);   # Right-Inf-Lat-Vent
        out  -= 2*(data==62);   # Right-vessel
        out  -= 2*(data==63);   # Right-choroid-plexus
        out  -= 2*(data==14);   # 3rd-Ventricle
        out  -= 2*(data==15);   # 4th-Ventricle
        out  -= 2*(data==72);   # 5th-Ventricle
        out  -= 2*(data==24);   # CSF
        
        nifti_out = nib.Nifti1Image(out, nifti.affine, nifti.header)
        nifti_out.header.set_data_dtype(np.int16)
        nib.save(nifti_out, sys.argv[2])
   
    
if __name__ == "__main__":
    main();
    
