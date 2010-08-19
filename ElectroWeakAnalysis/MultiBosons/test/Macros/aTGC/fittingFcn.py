import ROOT

class QuadFcn2:
    def __call__(self,In, Par):
        X = In[0]
        Y = In[1]
        return Par[0] + Par[1]*X + Par[2]*Y +  Par[3]*X*Y + Par[4]*X*X + Par[5]*Y*Y
