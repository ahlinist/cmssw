*............................................................................*
*............................................................................*
*..... GLOBAL     StaGen    parameters                                       *
*. . . . . .  INTEGER parametrs . . . . . . . . . . . . . . . . . . . . . .  *
*..Ipar(1)      ! internal StaGen code of the process                        *
*....  Black Hole production, Greg Landberg                                  *
*  Ipar(1) = 101 :  BH production, Greg Landberg                             *
*....  GRAVITON(G*) production,     Konstantin Matchev                       *
*  Ipar(1) = 111 : q + q ->     g + Graviton                                 *
*  Ipar(1) = 112 : q + g ->     q + Graviton                                 *
*  Ipar(1) = 113 : g + g ->     g + Graviton                                 *
*  Ipar(1) = 114 : q + q -> gamma + Graviton                                 * 
*.... GRAVITON(ADD) into gamma&gamma and l+ l-                               *
*  Ipar(1) = 120 : q + q~ -> l+    l-    (SM)                                *
*  Ipar(1) = 121 : q + q~ -> l+    l-    (ADD)                               *
*  Ipar(1) = 122 : q + q~ -> l+    l-    (SM+ADD)                            *
*  Ipar(1) = 123 : g + g  -> l+    l-    (ADD)                               *
*  Ipar(1) = 124 : q q~ + g g -> l+ l-   (SM+ADD)                            *
*......                                                                      *
*  Ipar(1) = 125 : q + q~ -> gamma gamma      (SM)                           *
*  Ipar(1) = 126 : q + q~ -> gamma gamma      (ADD)                          *
*  Ipar(1) = 127 : q + q~ -> gamma gamma      (SM+ADD)                       *
*  Ipar(1) = 128 : g + g  -> gamma gamma      (ADD)                          *
*  Ipar(1) = 129 : q q~ +  g g -> gamma gamma (SM+ADD)                       *
*....  EDDE,                        Roman Ryutin                             * 
*  Ipar(1) = 200 : P   P -> P Higgs  P   EDDE model                          * 
*  Ipar(1) = 201 : P   P -> P b bbar P   EDDE model                          * 
*............................................................................*
*  Ipar(2)      : num. of current event                                      *
*..Ipar(3)      : evolution scale type for PDF                               *
*..Ipar(4)      : evolution scale type for alpha_S                           *
*..Ipar(5)      : num. of events used for estimation of SigMax               *
*..Ipar(6)      : num. of events used for estimation of M**2                 *
*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . *
*  Ipar(15)     : number of current color                                    *
*..Ipar(20)     : continuation flag                                          *
*..Ipar(21)     : INIT flag                                                  *
*..Ipar(120)    : final leptons for ADD:                                     *
*                : 0=e+mu+tau, 1=e, 2=mu, 3=tau, 12=e+mu, 13=e+tau, 23=mu+tau*
*. . . . . . . . . . . . . .  . . . . . . . . . . . . . . . . . . . . . . .  *
*. . . . . .  DOUBLE PRECISION parameters . . . . . . . . . . . . . . . . .  *
*..Rpar(1)      : Ecm                                                        *
*..Rpar(2)      : S = Ecm**2                                                 *
*..Rpar(3)      : Evolution scale for PDF      [mass]                        *
*..Rpar(4)      : Evolution scale for alpha_s  [mass]                        *
*..Rpar(5)      : parameter for Q-scale evaluation for PDF                   *
*..Rpar(6)      : parameter for Q-scale evaluation for alpha_s               *
*.............. couplings ...................................................*
*..Rpar(10)     : 0.38939d0     transformation 1/GeV**2 to mb                *
*..Rpar(11)     : alpha_QED(Q2) QED electromagnetic coupling                 *
*..Rpar(12)     : 4.*pi*rpar(11)  = (electric charge)**2                     *
*..Rpar(13)     : sqrt(4.*pi*Rpar(11)) is electric charge                    *
*..Rpar(14)     : G_Fermi                                                    *
*..Rpar(15)     : alpha_QCD = QCD coupling used for event                    *
*..Rpar(16)     : 4*pi * Rpar(15) = (QCD charge)**2                          *
*..Rpar(17)     : sqrt(4*pi * Rpar(15)) = QCD charge                         *
*....                                                                        *
*..Rpar(21)     : sin(theta_W)                                               *
*..Rpar(22)     : cos(theta_W)                                               *
*..Rpar(23)     : sin(theta_W)**2                                            *
*..Rpar(24)     : cos(theta_W)**2                                            *
*..Rpar(25)     : sin(2*theta_W) = 2 * sin(theta_W) * cos(theta_W)           *
*..Rpar(26)     : g = g_weak  = e / sin(theta_W)                             *
*..Rpar(27)     : g_z         = e / sin(2*theta_W)                           *
*............................................................................*
*..Rpar(60)     : New Physics scale (1 TeV)                                  *
*............................................................................*
*..Rpar(100)    : differential cross section (in PB) for current event       *
*............................................................................*
*..Rpar(101)    : CKIN(1) = sqrt(Shat_MIN) for hard process                  *
*..Rpar(102)    : CKIN(2) = sqrt(Shat_MAX) for hard process                  *
*..Rpar(103)    : CKIN(3) = PThat_MIN                                        *
*..Rpar(104)    : CKIN(4) = PThat_MAX                                        *
*..Rpar(107)    : M(QQbar)_MIN  for EDDE b bbar production                   *
*............................................................................*
*...........  Graviton* production,  ADD scenario                            *
*..Rpar(121)    : M_exdim,                         ADD scenario              *
*..Rpar(122)    : n_exdim,  Graviton* production,  ADD scenario              *
*..Rpar(123)    : mKK mass of KK-excitation,       ADD scenario              *
*..Rpar(124)    : recommended PThat_MIN            ADD scenario              *
*..Rpar(125)    : factor_ADD                                                 *
*............................................................................*
*...........  Black Hole (BH) production                                     *
*  Rpar(131)    : Planck scale, GeV                                          *
*  Rpar(132)    : n extra dimensions                                         *
*  Rpar(133)    : BR(BH -> q qbar X)                                         *
*  Rpar(134)    : BR(BH -> g g    X) / 2                                     *
*  Rpar(135)    : BR(BH -> l l'   X)                                         *
*  Rpar(136)    : BR(BH -> gamma,W,Z X) + BR(BH -> Higgs  X)                 *
*  Rpar(137)    : BR(BH -> Higgs  X)                                         *
*............................................................................*
      Integer           Ipar     ! global TopRex integer parameters  
      double precision  Rpar     ! global TopRex real    parameters
      common /TXPAR/Ipar(200), Rpar(200)
      save   /TXPAR/
**********
