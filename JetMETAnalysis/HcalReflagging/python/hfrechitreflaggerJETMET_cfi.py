import FWCore.ParameterSet.Config as cms


# This is the HF PMT window reflagger
hfrechitReflaggerJETMET = cms.EDProducer('HFRecHitReflaggerJETMET',
    debug          = cms.untracked.int32(0),
    hfInputLabel       = cms.untracked.InputTag("hfreco"),
    hfFlagBit          = cms.untracked.int32(31),  # bit 31 is UserDefinedBit0; see RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h for full list
    hfBitAlwaysOn      = cms.untracked.bool(False),
    hfBitAlwaysOff     = cms.untracked.bool(False),


    # Short Fiber parameters

    # Specify whether to use PET for each short fiber.  If false, S9S1 will be used instead.
    # CAUTION!  S9S1 was developed only for long fibers, and may give nonsensical results for
    # short fibers!

    # Separate boolean given for each |ieta|
    hf_Short_Use_PET = cms.vint32([1,  #|ieta=29|
                                   1,  #|ieta=30|
                                   1,
                                   1,
                                   1,
                                   1,
                                   1,  #|ieta=35|
                                   1,
                                   1,
                                   1,
                                   1,
                                   1,  #|ieta=40|
                                   1,  #|ieta=41|
                                   ]),

    # PET Energy threshold for short fibers, stored as list of coefficients for polynomial expansion in ieta.
    # Polynomial expansion may proceed to arbitray powers of |ieta|.
    # Energy must be > [0] + [1]*(|ieta|) + [2]*(|ieta|^2) +...  to pass cut
                                           
    hf_Short_PET_Energy_Thresh = cms.PSet(
        hf_Short_PET_energyThresh29=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_PET_energyThresh30=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_PET_energyThresh31=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_PET_energyThresh32=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_PET_energyThresh33=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_PET_energyThresh34=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_PET_energyThresh35=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_PET_energyThresh36=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_PET_energyThresh37=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_PET_energyThresh38=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_PET_energyThresh39=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_PET_energyThresh40=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_PET_energyThresh41=cms.vdouble([129.9,-6.61,0.1153]),
    ),

    # ET threshold for short fibers, stored as list of coefficients for polynomial expansion in ieta
    hf_Short_PET_ET_Thresh = cms.PSet(
        hf_Short_PET_ETThresh29=cms.vdouble([0]),
        hf_Short_PET_ETThresh30=cms.vdouble([0]),
        hf_Short_PET_ETThresh31=cms.vdouble([0]),
        hf_Short_PET_ETThresh32=cms.vdouble([0]),
        hf_Short_PET_ETThresh33=cms.vdouble([0]),
        hf_Short_PET_ETThresh34=cms.vdouble([0]),
        hf_Short_PET_ETThresh35=cms.vdouble([0]),
        hf_Short_PET_ETThresh36=cms.vdouble([0]),
        hf_Short_PET_ETThresh37=cms.vdouble([0]),
        hf_Short_PET_ETThresh38=cms.vdouble([0]),
        hf_Short_PET_ETThresh39=cms.vdouble([0]),
        hf_Short_PET_ETThresh40=cms.vdouble([0]),
        hf_Short_PET_ETThresh41=cms.vdouble([0])
    ),

    # Specify R cut for each hf_Short Ieta
    # This is a list of coefficients for a polynomial expansion in energy.
    # (If hf_Short_Rcut_parameterizeET == True, the expansion is made in powers of ET, rather than energy.)
    # Default cut is independent of energy:  R = 0.98*pow(Energy,0)
                                           
    hf_Short_Rcut=cms.PSet(
        hf_Short_Rcut_parameterizeET=cms.untracked.bool(False),
        hf_Short_Rcut_values29=cms.vdouble([0.98]),
        hf_Short_Rcut_values30=cms.vdouble([0.98]),
        hf_Short_Rcut_values31=cms.vdouble([0.98]),
        hf_Short_Rcut_values32=cms.vdouble([0.98]),
        hf_Short_Rcut_values33=cms.vdouble([0.98]),
        hf_Short_Rcut_values34=cms.vdouble([0.98]),
        hf_Short_Rcut_values35=cms.vdouble([0.98]),
        hf_Short_Rcut_values36=cms.vdouble([0.98]),
        hf_Short_Rcut_values37=cms.vdouble([0.98]),
        hf_Short_Rcut_values38=cms.vdouble([0.98]),
        hf_Short_Rcut_values39=cms.vdouble([0.98]),
        hf_Short_Rcut_values40=cms.vdouble([0.98]),
        hf_Short_Rcut_values41=cms.vdouble([0.98]),
    ),

    # AGAIN, WE DON'T RECOMMEND USING S9S1 for short fibers!

    # Energy threshold is parameterized as polynomial in |ieta|, as above
    hf_Short_S9S1_Energy_Thresh = cms.PSet(
        hf_Short_S9S1_energyThresh29=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_S9S1_energyThresh30=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_S9S1_energyThresh31=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_S9S1_energyThresh32=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_S9S1_energyThresh33=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_S9S1_energyThresh34=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_S9S1_energyThresh35=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_S9S1_energyThresh36=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_S9S1_energyThresh37=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_S9S1_energyThresh38=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_S9S1_energyThresh39=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_S9S1_energyThresh40=cms.vdouble([129.9,-6.61,0.1153]),
        hf_Short_S9S1_energyThresh41=cms.vdouble([129.9,-6.61,0.1153]),
    ),

    # ET threshold is parameterized as polynomial in |ieta|, as above
                                           
    hf_Short_S9S1_ET_Thresh = cms.PSet(
        hf_Short_S9S1_ETThresh29=cms.vdouble([0]),
        hf_Short_S9S1_ETThresh30=cms.vdouble([0]),
        hf_Short_S9S1_ETThresh31=cms.vdouble([0]),
        hf_Short_S9S1_ETThresh32=cms.vdouble([0]),
        hf_Short_S9S1_ETThresh33=cms.vdouble([0]),
        hf_Short_S9S1_ETThresh34=cms.vdouble([0]),
        hf_Short_S9S1_ETThresh35=cms.vdouble([0]),
        hf_Short_S9S1_ETThresh36=cms.vdouble([0]),
        hf_Short_S9S1_ETThresh37=cms.vdouble([0]),
        hf_Short_S9S1_ETThresh38=cms.vdouble([0]),
        hf_Short_S9S1_ETThresh39=cms.vdouble([0]),
        hf_Short_S9S1_ETThresh40=cms.vdouble([0]),
        hf_Short_S9S1_ETThresh41=cms.vdouble([0])
    ),

    # Slope_Intercept should be a list with two elements:  [slope, intercept]
    # If no intercept is specified, it will be calculated directly within the code
    # (In base S9S1 method, the intercept depends on the energy threshold for each ieta:  intercept = -1*slope*(Energy_threshold) )
    hf_Short_S9S1_Slope_Intercept = cms.PSet(
        hf_Short_S9S1_SlopeIntercept29=cms.vdouble([-9999]),
        hf_Short_S9S1_SlopeIntercept30=cms.vdouble([0.0164905]),
        hf_Short_S9S1_SlopeIntercept31=cms.vdouble([0.0238698]),
        hf_Short_S9S1_SlopeIntercept32=cms.vdouble([0.0321383]),
        hf_Short_S9S1_SlopeIntercept33=cms.vdouble([0.041296]),
        hf_Short_S9S1_SlopeIntercept34=cms.vdouble([0.0513428]),
        hf_Short_S9S1_SlopeIntercept35=cms.vdouble([0.0622789]),
        hf_Short_S9S1_SlopeIntercept36=cms.vdouble([0.0741041]),
        hf_Short_S9S1_SlopeIntercept37=cms.vdouble([0.0868186]),
        hf_Short_S9S1_SlopeIntercept38=cms.vdouble([0.100422]),
        hf_Short_S9S1_SlopeIntercept39=cms.vdouble([0.135313]),
        hf_Short_S9S1_SlopeIntercept40=cms.vdouble([0.136289]),
        hf_Short_S9S1_SlopeIntercept41=cms.vdouble([0.0589927]),
    ),


    ###################################################################################

    # Long Fiber parameters -- same format as short fibers

    # Specify whether to use PET for each long fiber.  If false, S9S1 will be used instead.

    # Separate boolean given for each |ieta|
    hf_Long_Use_PET = cms.vint32([1,  #|ieta=29|
                                  0,  #|ieta=30|
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,  #|ieta=35|
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,  #|ieta=40|
                                  0,  #|ieta=41|
                                  ]),


    hf_Long_PET_Energy_Thresh = cms.PSet(
        hf_Long_PET_energyThresh29=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_PET_energyThresh30=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_PET_energyThresh31=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_PET_energyThresh32=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_PET_energyThresh33=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_PET_energyThresh34=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_PET_energyThresh35=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_PET_energyThresh36=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_PET_energyThresh37=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_PET_energyThresh38=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_PET_energyThresh39=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_PET_energyThresh40=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_PET_energyThresh41=cms.vdouble([162.4,-10.19,0.21]),
    ),

    hf_Long_PET_ET_Thresh = cms.PSet(
        hf_Long_PET_ETThresh29=cms.vdouble([0]),
        hf_Long_PET_ETThresh30=cms.vdouble([0]),
        hf_Long_PET_ETThresh31=cms.vdouble([0]),
        hf_Long_PET_ETThresh32=cms.vdouble([0]),
        hf_Long_PET_ETThresh33=cms.vdouble([0]),
        hf_Long_PET_ETThresh34=cms.vdouble([0]),
        hf_Long_PET_ETThresh35=cms.vdouble([0]),
        hf_Long_PET_ETThresh36=cms.vdouble([0]),
        hf_Long_PET_ETThresh37=cms.vdouble([0]),
        hf_Long_PET_ETThresh38=cms.vdouble([0]),
        hf_Long_PET_ETThresh39=cms.vdouble([0]),
        hf_Long_PET_ETThresh40=cms.vdouble([0]),
        hf_Long_PET_ETThresh41=cms.vdouble([0])
    ),

    # Specify R cut for each hf_Long Ieta
    hf_Long_Rcut=cms.PSet(
        hf_Long_Rcut_parameterizeET=cms.untracked.bool(False),
        hf_Long_Rcut_values29=cms.vdouble([0.98]),
        hf_Long_Rcut_values30=cms.vdouble([0.98]),
        hf_Long_Rcut_values31=cms.vdouble([0.98]),
        hf_Long_Rcut_values32=cms.vdouble([0.98]),
        hf_Long_Rcut_values33=cms.vdouble([0.98]),
        hf_Long_Rcut_values34=cms.vdouble([0.98]),
        hf_Long_Rcut_values35=cms.vdouble([0.98]),
        hf_Long_Rcut_values36=cms.vdouble([0.98]),
        hf_Long_Rcut_values37=cms.vdouble([0.98]),
        hf_Long_Rcut_values38=cms.vdouble([0.98]),
        hf_Long_Rcut_values39=cms.vdouble([0.98]),
        hf_Long_Rcut_values40=cms.vdouble([0.98]),
        hf_Long_Rcut_values41=cms.vdouble([0.98]),
    ),

    hf_Long_S9S1_Energy_Thresh = cms.PSet(
        hf_Long_S9S1_energyThresh29=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_S9S1_energyThresh30=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_S9S1_energyThresh31=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_S9S1_energyThresh32=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_S9S1_energyThresh33=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_S9S1_energyThresh34=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_S9S1_energyThresh35=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_S9S1_energyThresh36=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_S9S1_energyThresh37=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_S9S1_energyThresh38=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_S9S1_energyThresh39=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_S9S1_energyThresh40=cms.vdouble([162.4,-10.19,0.21]),
        hf_Long_S9S1_energyThresh41=cms.vdouble([162.4,-10.19,0.21]),
    ),

    hf_Long_S9S1_ET_Thresh = cms.PSet(
        hf_Long_S9S1_ETThresh29=cms.vdouble([0]),
        hf_Long_S9S1_ETThresh30=cms.vdouble([0]),
        hf_Long_S9S1_ETThresh31=cms.vdouble([0]),
        hf_Long_S9S1_ETThresh32=cms.vdouble([0]),
        hf_Long_S9S1_ETThresh33=cms.vdouble([0]),
        hf_Long_S9S1_ETThresh34=cms.vdouble([0]),
        hf_Long_S9S1_ETThresh35=cms.vdouble([0]),
        hf_Long_S9S1_ETThresh36=cms.vdouble([0]),
        hf_Long_S9S1_ETThresh37=cms.vdouble([0]),
        hf_Long_S9S1_ETThresh38=cms.vdouble([0]),
        hf_Long_S9S1_ETThresh39=cms.vdouble([0]),
        hf_Long_S9S1_ETThresh40=cms.vdouble([0]),
        hf_Long_S9S1_ETThresh41=cms.vdouble([0])
    ),

    # If no intercept is specified, it will be calculated directly within the code
    hf_Long_S9S1_Slope_Intercept = cms.PSet(
        hf_Long_S9S1_SlopeIntercept29=cms.vdouble([-9999]),
        hf_Long_S9S1_SlopeIntercept30=cms.vdouble([0.0164905]),
        hf_Long_S9S1_SlopeIntercept31=cms.vdouble([0.0238698]),
        hf_Long_S9S1_SlopeIntercept32=cms.vdouble([0.0321383]),
        hf_Long_S9S1_SlopeIntercept33=cms.vdouble([0.041296]),
        hf_Long_S9S1_SlopeIntercept34=cms.vdouble([0.0513428]),
        hf_Long_S9S1_SlopeIntercept35=cms.vdouble([0.0622789]),
        hf_Long_S9S1_SlopeIntercept36=cms.vdouble([0.0741041]),
        hf_Long_S9S1_SlopeIntercept37=cms.vdouble([0.0868186]),
        hf_Long_S9S1_SlopeIntercept38=cms.vdouble([0.100422]),
        hf_Long_S9S1_SlopeIntercept39=cms.vdouble([0.135313]),
        hf_Long_S9S1_SlopeIntercept40=cms.vdouble([0.136289]),
        hf_Long_S9S1_SlopeIntercept41=cms.vdouble([0.0589927]),
    )
)
