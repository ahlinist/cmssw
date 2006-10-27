      PROGRAM MaterialMixtures
C     ========================

      IMPLICIT NONE

      Integer ISTAT,i,j,k,l

      Character*1 Coding, Code
      Character*20 Filename,OutFile,InFile,tzfile,x0file
      Character*120 inputstring

      Integer Nmix, Ndiv,LunOut,LunIn,Index, Luntz,Lunx0

C...Common Block .................................................
      Integer MaxDiv
      Parameter (MaxDiv=30)
      Character*40 MixtureName, GMIXName
      Character*30 Comment(MaxDiv),Material(MaxDiv)
      Character*3 Type(MaxDiv)
      Real Volume(MaxDiv), Mult(MaxDiv),
     +     Density(MaxDiv),Radl(MaxDiv),MCVolume,MCArea
      Common /MatMix/ Comment, Material,Volume,Mult,Density,Radl,
     +                MCVolume,MCArea,MixtureName,GMIXName,Type
C.................................................................


      External Lenocc
      Integer Lenocc      


C... initialization

         

C---> read in the input file from standard input
      write(*,*) " Which file do you want me to open ?"
      read(*,*) Filename
      InFile = Filename(1:LENOCC(Filename))//".in"
      OutFile = Filename(1:LENOCC(Filename))//".tex" 
      tzfile  = Filename(1:LENOCC(Filename))//".titles"
      x0file  = Filename(1:LENOCC(Filename))//".x0"

      write(*,*) Filename, InFile, OutFile

      LunIn = 23
      LunOut = 24
      Luntz = LunOut + 1
      Lunx0 = LunOut + 2
      open(unit=LunIn,file=InFile,status="OLD")
      open(unit=LunOut,file=OutFile,status="REPLACE")
      open(unit=Luntz,file=tzfile,status="REPLACE")
      open(unit=Lunx0,file=x0file,status="REPLACE")
      call LatexSetup(LunOut)


C---> Big loop over input file

      Nmix = 0
      Ndiv = 0
      do l = 1, 10000     !! file should not have more lines than that

         read(LunIn,'(A)',END=20) inputstring
C         write(*,*) inputstring
         
C...     first check for start of new mixture

         Coding = inputstring(1:1)
C         write(*,*) "Coding ", Coding
         if ( Coding.eq.'#') then   ! the next mixture starts
            if (Nmix.gt.0) then     ! do everything for the last mixture 
               call MakeMixture(NMix,Ndiv,LunOut)
C               write(*,*) "Nmix ", Nmix
C               write(*,*) "Mixture Name:  ", MixtureName, GMixName
C               do j = 1, Ndiv
C                  write(*,*) Code, Index, Comment(j), Material(j),
C     +                 Volume(j), Mult(j), MCVolume
C               enddo
            endif
C... reset everything
            Call ClearCommon
            Nmix = Nmix + 1
            Ndiv = 0
            read(inputstring,*)  Code,MixtureName,GMIXName,
     +           MCVolume,MCArea

         elseif ( Coding.eq.'*') then        ! components
            Ndiv = Ndiv + 1
            read(inputstring,*) Code, Index, Comment(Ndiv), 
     +           Material(Ndiv),Volume(Ndiv), Mult(Ndiv), Type(Ndiv)
            call MatchMaterial(Ndiv)
            if(Ndiv.ne.Index)  write(*,*) 
     +         "******* Inconsistency reading in ",InFile," ******"
         endif

      enddo
 20   continue


C      write(LunOut,*) "\\end{center}"
      write(LunOut,*) "\\end{document}"

      close(LunIn)
      close(LunOut)
      close(Luntz)
      close(Lunx0)
       
C... write out little latex/dvips script
      open(30,file="do",status="REPLACE")
      write(30,*) "latex ",Filename(1:LENOCC(Filename))
      write(30,*) "dvips ",Filename(1:LENOCC(Filename)),
     +     " -o",Filename(1:LENOCC(Filename)),".ps"
      write(30,*) "gv ",Filename(1:LENOCC(Filename))," &"
      close(30)

      write(*,*) "--> I made ",Filename(1:LENOCC(Filename)),
     +   "  for you. Type ''do'' to see it " 

      end


C----------------------------------------------------------------
      
      Subroutine MatchMaterial(Index)
C     ========================

      Implicit None

      Integer Index, Istat, I,J

      Integer MaxPure
      Parameter (MaxPure=350)
      Integer NPure,match
      CHARACTER*25 PureName(MaxPure)
      REAL Pureweight(MaxPure), Purenumber(MaxPure), Puredens(MaxPure),
     +     PureX0(MaxPure),Pureabsl(MaxPure) 
      SAVE NPure, Pureweight, Purenumber, Puredens, PureX0, Pureabsl

      Character*30 string,teststring

      Logical DEBUG,FIRST
      DATA FIRST /.TRUE./
      DATA DEBUG /.TRUE./

      EXTERNAL LENOCC
      Integer LENOCC


C...Common Block .................................................
      Integer MaxDiv
      Parameter (MaxDiv=30)
      Character*40 MixtureName, GMIXName
      Character*30 Comment(MaxDiv),Material(MaxDiv)
      Character*3 Type(MaxDiv)
      Real Volume(MaxDiv), Mult(MaxDiv),
     +     Density(MaxDiv),Radl(MaxDiv),MCVolume,MCArea
      Common /MatMix/ Comment, Material,Volume,Mult,Density,Radl,
     +                MCVolume,MCArea,MixtureName,GMIXName,Type
C.................................................................


C... read in pure material file

      if (FIRST) then

         open(unit=22,file="pure_materials.input",status="OLD",
     +        IOSTAT=istat)
         
         if(istat.ne.0) then
            write(*,*) "Pure Materials input file could not be opened",
     +           " - I quit"
            stop
         endif

         Npure = 0
         do i=1, MaxPure
            read(22,*,END=10) PureName(i), Pureweight(i), 
     +           PureNumber(i),PureDens(i), PureX0(i),Pureabsl(i)
            Npure = Npure + 1
         enddo
 10      continue

         close(22)

C... read in mixed material file

         open(unit=22,file="mixed_materials.input",status="OLD",
     +        IOSTAT=istat)
         
         if(istat.ne.0) then
            write(*,*) "Mixed Materials input file could not be opened",
     +           " - I quit"
            stop
         endif

         do i=Npure+1, MaxPure
            read(22,*,END=20) PureName(i), Pureweight(i), 
     +           PureNumber(i),PureDens(i), PureX0(i),Pureabsl(i)
            Npure = Npure + 1
         enddo
 20      continue

         close(22)
C
         if (debug) then
            write(*,*) "Number of pure materials:  ", Npure
            write(*,*) "Material name            ", "A        ",
     +           "Z       ",
     +           "dens    ", "X_0        ","abs_l       "
            do j= 1, NPure
               write(*,200) PureName(j), Pureweight(j), 
     +           PureNumber(j),PureDens(j), PureX0(j),Pureabsl(j)
            enddo
         endif
 200     Format(A20,F10.5,F7.0,3F10.5)

         FIRST = .FALSE.
      endif

C---> try to match material here !

      String = Material(Index)

      if (DEBUG) write(*,*) 'Matching now ', String

      match = 0
      Do i = 1,NPure
         teststring = PureName(i)
         if(teststring(1:LENOCC(teststring)).eq.
     +        string(1:LENOCC(string))) then
            if (debug)  write(*,200) PureName(i), Pureweight(i), 
     +           PureNumber(i), PureDens(i), PureX0(i),Pureabsl(i)           
            match = 1
C... set density and radiation lenght
            Density(Index) = Puredens(I)
            Radl(Index) = PureX0(I)
       endif
      enddo

      if (match.ne.1)then
         write(*,*) "Couldn't find match for material  ",
     +        Material(Index)
         write(*,*) "Exiting !!"
         stop
      else
         if(Radl(Index).le.0.) then
            write(*,*) "Radiation length is zero for material ",
     +           Material(Index)
         endif
         if(Density(Index).le.0) then
             write(*,*) "Density is zero for material ",
     +           Material(Index)
          endif
      endif

      return
      end

C--------------------------------------------------------------

      Subroutine MakeMixture(Nmix,NMat,LUN)
C     =====================================

      Implicit None

C...Common Block .................................................
      Integer MaxDiv
      Parameter (MaxDiv=30)
      Character*40 MixtureName, GMIXName
      Character*30 Comment(MaxDiv),Material(MaxDiv)
      Character*3 Type(MaxDiv)
      Real Volume(MaxDiv), Mult(MaxDiv),
     +     Density(MaxDiv),Radl(MaxDiv),MCVolume,MCArea
      Common /MatMix/ Comment, Material,Volume,Mult,Density,Radl,
     +                MCVolume,MCArea,MixtureName,GMIXName,Type
C.................................................................

      Integer NMat, i, j, k,LUN,NMix,LUNTZ,NTZ,Lunx0

      Real TVOL,TDEN,TRAD,Weight,PVOL(MaxDiv),PWeight(MaxDiv)
      Real ws(MaxDiv),tmp,PRAD(MaxDiv),Norm,Ndens,NRadl,PRadl

      Real PSUP,PSEN,PCAB,PCOL,PELE

      Character*30 string,string1,string2,stringmatname

      Character*30 TZName(MaxDiv)
      Character*32 tzstring
      Real         TZVol(MaxDiv), TZVolTot

      External LENOCC
      Integer LENOCC
      
C..................................................................

C..initialize
      TVOL = 0.     ! compound volume
      TDEN = 0.     ! compound density
      TRAD = 0.     ! compound radiation length
      Weight = 0.   ! Total weight
      call VZERO(PVOL,MaxDiv)
      call VZERO(Pweight,MaxDiv)
      call VZERO(ws,MaxDiv)
      call VZERO(PRAD,MaxDiv)
      call VZERO(TZVol,MaxDiv)
      tmp = 0.

* total volume
      do i=1, NMat
         Volume(i) = Mult(i)*Volume(i)
         TVOL = TVOL + Volume(i)
      enddo

      if (tvol.le.0.) return
* percentual volume and total density
      do i=1,NMat
         PVOL(i) = Volume(i)/TVOL
         TDEN = TDEN + PVOL(i)*Density(i)
      enddo

* total weight
      Weight = TDEN * TVOL

      do j = 1,NMat
* percentual weight
         if(Volume(j).gt.0.) then
            PWeight(j) = Density(j)*Volume(j)/Weight
* weight for X0 calculation (pweight/(density*radl))
            ws(j) =  Pweight(j)/(Density(j)*Radl(j))
            tmp = tmp + ws(j)
         endif
      enddo
      
* radiation length of compound
      TRAD = 1/(tmp*TDEN)

* contribution to compound X0
      do k = 1,NMat
         PRAD(k) = ws(k)*TRAD*TDEN
      enddo

* Normalization factor Mixture/MC volume
      if (MCVolume.gt.0.) then
         Norm = TVOL/MCVolume
      else
         Norm = 1.
      endif

* Normalized density and radiation length

      ndens = TDEN*Norm
      NRadl = TRAD / norm

* percentual radiation length of compound (if area is given)
      if (MCArea.gt.0) then
         PRadl = MCVolume/(MCArea*NRadl)
      endif

C---> separate contributions to X_0 by type
      PSUP = 0.
      PSEN = 0.
      PCAB = 0.
      PCOL = 0.
      PELE = 0.
      do i = 1, NMat
         if(Type(i).eq."SUP") then
            PSUP = PSUP + PRAD(i)
         elseif (Type(i).eq."SEN") then
            PSEN = PSEN + PRAD(i)
         elseif (Type(i).eq."CAB") then
            PCAB = PCAB + PRAD(i) 
         elseif (Type(i).eq."COL") then
            PCOL = PCOL + PRAD(i) 
         elseif (Type(i).eq."ELE") then
            PELE = PELE + PRAD(i) 
         else
            write(*,*) "No grouping given for material ",
     +                 Material(i)
         endif
      enddo

C---> write out the results ..................

      stringmatname = GMIXName
      call LatexUnderscore(stringmatname)
      write(LUN,1000) Nmix,MixtureName,stringmatname
 1000 Format('\\subsection*{\\underline{',I3,2X,A40,2X,
     +     '(Material name: ',A40,')',' }}')
      
      write(LUN,*) "\\begin{table}[h]"
      write(LUN,*) "\\begin{tabular}{rlrrr}"
      write(LUN,*) "\\hline\\hline"
      write(LUN,*) " & Item & \\% Volume & \\% Weight & ",
     +     "\\% Total X0  \\","\\"
      write(LUN,*) "\\hline\\hline"
      
      do k=1,NMat
         string = Material(k)
         call LatexUnderscore(string)
         write(LUN,1001) k, string(1:LENOCC(string)),100.*PVOL(k),
     +        100.*Pweight(k),100.*PRAD(k)
         write(LUN,*) "\\hline"
      enddo
 1001 Format(1X,I4,2X,' & ',A20,' & ',2(1X,F8.3,' & '),1X,F8.3,
     +     '\\','\\')
      
      write(LUN,*) " & & & & \\","\\"
      write(LUN,1002) "Mixture density",TDEN
      write(LUN,1002) "Norm. mixture density",Ndens
      write(LUN,1002) "Mixture Volume",TVOL
      write(LUN,1002) "MC Volume",MCVolume
      write(LUN,1002) "Normalization factor",Norm
      write(LUN,1002) "Mixture X0 (cm)", TRAD
      write(LUN,1002) "Norm. Mixture X0 (cm)",NRadl
      if (MCArea.gt.0) then
         write(LUN,1004) "Norm. Mixture X0 (\\%)",100*PRadl
      endif
      write(LUN,1002) "Total weight (g)",weight

 1002 Format(" & ",A25," & ",F11.5," && \\","\\")
 1004 Format(" & ",A25," & ",F11.3," && \\","\\")

      write(LUN,*) " & & & & \\","\\"
      write(LUN,*) " & & & & \\","\\"
   
      write(LUN,*) " & Components & Material & Density & X0 (cm)\\","\\"
      write(LUN,*) "\\hline"
      
      do i = 1,NMat
         string1 = Comment(i)
         string2 = Material(i)
         call LatexUnderscore(string1)
         call LatexUnderscore(string2)
         write(LUN,1003) i,string1(1:LENOCC(string1)),
     +        string2(1:LENOCC(string2)),Density(i), Radl(i)
      enddo
 1003 format(1X,I4," & ",A22,"&",A22,"&",F8.3," & ",F8.3," \\","\\")

      write(LUN,*) " & & & & \\","\\"
      write(LUN,1006) "\\underline{X0 contribution}"
      write(LUN,1005) "Support: ",PSUP
      write(LUN,1005) "Sensitive: ",PSEN
      write(LUN,1005) "Cables: ",PCAB
      write(LUN,1005) "Cooling: ",PCOL
      write(LUN,1005) "Electronics: ", PELE

 1005 Format(" & ",A25," & ",F5.3," && \\","\\")
 1006 Format(" & ",A30," &   & & \\","\\")

      write(LUN,*) "\\end{tabular}"
      write(LUN,*) "\\end{table}"
      write(LUN,*) "\\clearpage"

C----> now write out a pseudo title file

      LUNTZ = LUN+1

C * first add volumes of same material
      
      Ntz = 0

      do 500 i = 1, NMat  
C.. see if there's a match    
         do j = 1, Ntz
            if(Material(i)(1:LENOCC(Material(i))).eq.
     +           TZName(j)(1:LENOCC(TZName(j))) ) then
               TZVol(j) = TZVol(j) + PVol(i)
               go to 500
            endif
         enddo
         Ntz = Ntz + 1
         TZName(Ntz) = material(i)
C         write(*,*) "Ntz increased: ",NTz, TZName(Ntz)
         TZVol(Ntz)  = PVol(i)
 500  continue

      TZVolTot = 0.
      do i = 1, Ntz
         TZVolTot = TZVolTot + TZVol(i)
      enddo
      if( abs(TZVolTot-1.) .gt. 1.E-6) then
         write(*,*) "Percentual Volumes don't add up to 1 !!"
      endif

C      write(*,*) "NTZ: ", Ntz
C      do i =1, Ntz
C         write(*,*) TZName(i)
C      enddo

      tzstring = '"'//GMIXName(1:LENOCC(GMIXName))//'"'
      write(LUNTZ,1010) tzstring,-1*Ntz, ndens
      do j = 1, Ntz
         tzstring = '"'//TZName(j)(1:LENOCC(TZName(j)))//'"'
         write(LUNTZ,1011) tzstring, -100.*TZVol(j)
      enddo

 1010 Format(7X,A20,I3,4X,F8.3)
 1011 Format(10X,A22,F8.3)

C--> and x0 contributions into a separate file

      Lunx0 = LUN+2

C     Original
C     tzstring = '"'//GMIXName(1:LENOCC(GMIXName))//'"'
C     if(PSUP.gt.0.) write(Lunx0,1012) tzstring,'" SUP"',PSUP
C     if(PSEN.gt.0.) write(Lunx0,1012) tzstring,'" SEN"',PSEN
C     if(PCAB.gt.0.) write(Lunx0,1012) tzstring,'" CAB"',PCAB
C     if(PCOL.gt.0.) write(Lunx0,1012) tzstring,'" COL"',PCOL
C     if(PELE.gt.0.) write(Lunx0,1012) tzstring,'" ELE"',PELE
C     write(Lunx0,*) "   "
C     1012 Format(1X,A23,A6,1X,F5.3)
C     

C     rr
      tzstring = GMIXName(1:LENOCC(GMIXName))
      write(Lunx0,1012) tzstring,PSUP,PSEN,PCAB,PCOL,PELE
      write(Lunx0,*) "   "
 1012 Format(A23,1X,F5.3,1X,F5.3,1X,F5.3,1X,F5.3,1X,F5.3)
C     rr
      
      return
      end

C----------------------------------------------------------------
      
      Subroutine ClearCommon
C     ----------------------

      Implicit None

      Integer I

C...Common Block .................................................
      Integer MaxDiv
      Parameter (MaxDiv=30)
      Character*40 MixtureName, GMIXName
      Character*30 Comment(MaxDiv),Material(MaxDiv)
      Character*3 Type(MaxDiv)
      Real Volume(MaxDiv), Mult(MaxDiv),
     +     Density(MaxDiv),Radl(MaxDiv),MCVolume,MCArea
      Common /MatMix/ Comment, Material,Volume,Mult,Density,Radl,
     +                MCVolume,MCArea,MixtureName,GMIXName,Type
C.................................................................

      do i=1, MaxDiv
         Comment(i) = "  "
         Material(i) = "  "
         Type(i)     = "  "
      enddo

      Call VZERO(Volume,MaxDiv)
      Call VZERO(Mult,MaxDiv)
      Call VZERO(Density,MaxDiv)
      Call VZERO(Radl,MaxDiv)
      MCVolume = 0.
      MCArea = 0.
      MixtureName = " "
      GMIXName = " "

      return 
      end

C------------------------------------------------------------------

      Subroutine LatexSetup(LUN)
C     ==========================
      
      Implicit None
      Integer LUN
C--
      write(LUN,*) "\\documentstyle[11pt]{article}"
      write(LUN,*) "\\pagestyle{empty}"
      write(LUN,*) "\\renewcommand{\\baselinestretch}{1.1}"
      write(LUN,*) "\\parskip4pt"
      write(LUN,*) "\\setlength{\\textwidth}{18cm}"
      write(LUN,*) "\\setlength{\\textheight}{31.cm}"     
C      write(LUN,*) "\\addtolength{\\oddsidemargin}{-1.5cm}"
      write(LUN,*) "\\addtolength{\\topmargin}{-2cm}"
      write(LUN,*) "\\begin{document}"
        
      return
      end
      


      Subroutine LatexUnderscore(stringname)
C     =======================================
      Implicit None
      Character*30 stringname,stringtemp
      Integer      k,maxunderscore,findunderscore,findspace
      Integer      underscorefound
      
      stringtemp = stringname
      findunderscore = 0
      k = 0
      maxunderscore = 5                                                  At most maxunderscore '_' searched
      underscorefound = 0
      
C     Avoid LaTeX errors when compiling names with '_'
c     write(*,*) k,stringname,stringtemp
      do k=1,maxunderscore
         findunderscore = INDEX(stringtemp,'_')
         if(findunderscore.ne.0) then
            underscorefound = underscorefound + 1
            if(k.eq.1) then
               stringname = stringtemp(1:findunderscore-1) // '\\'
     +              // stringtemp(findunderscore:findunderscore)
            else
               findspace = INDEX(stringname,' ')
               stringname = stringname(1:findspace-1)
     +              // stringtemp(1:findunderscore-1) // '\\'
     +              // stringtemp(findunderscore:findunderscore)
            endif
            stringtemp = stringtemp(findunderscore+1:)
         endif
c     write(*,*) k,stringname,stringtemp
      enddo
      if(underscorefound.ne.0) then
         findspace = INDEX(stringname,' ')
         stringname = stringname(1:findspace-1) // stringtemp
      endif
c     write(*,*) k,stringname,stringtemp
      return
      end
C     
      











