def axis_limits(Par,file):
    #Double_t A,B,C,C1;
    
    # set limits on x axis by setting y=0
    A=Par[4] # should be 3 for QuadFcn
    if (A==0): A=0.00000001
    B=Par[1]
    C1=1.92
    C=B*B/4.0/A-C1
    SQ=B*B-4.0*A*C
    RT=ROOT.sqrt(SQ)
    A1=(-B-RT)/2.0/A
    A2=(-B+RT)/2.0/A
    St = (file.name[:-4]).split("_")
    print St
    print St[0], St[1], St[2],"Limits on h3: %.2f < h3 < %.2f"%(A1,A2)
    
    x1 = A1
    x2 = A2
    
    # set limits on y axis by setting x=0
    A=Par[5]; # should be 4 for QuadFcn
    if (A==0): A=0.00000001
    B=Par[2]
    C=B*B/4.0/A-C1
    SQ=B*B-4.0*A*C
    RT=ROOT.sqrt(SQ)
    A1=(-B-RT)/2.0/A
    A2=(-B+RT)/2.0/A
    print St[0], St[1], St[2],"Limits on h4: %.4f < h4 < %.4f"%(A1,A2)
    
    y1 = A1
    y2 = A2
    
    return [x1,x2,y1,y2]
