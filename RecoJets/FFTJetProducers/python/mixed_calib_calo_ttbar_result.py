# Generated Tue Jul 05 01:58:28 PM CDT 2011 by user igv with script generate_pileup_calibration.tcl
import FWCore.ParameterSet.Config as cms

uncertaintyZones = (-1.0e21, -1.0e20, 0.000102969119325, 0.000462858239189, 0.00133660959546)

calibrationCurve = cms.PSet(
   Class = cms.string("LinInterpolatedTable1D"),
   xmin = cms.double(0.000102969119325),
   xmax = cms.double(0.015941093673),
   leftExtrapolationLinear = cms.bool(False),
   rightExtrapolationLinear = cms.bool(True),
   data = cms.vdouble((0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0106699466705, 0.0239287111908, 0.0361104235053, 0.0512956343591, 0.0685216262937, 0.0840348005295, 0.0984199643135, 0.118270792067, 0.135448351502, 0.156822130084, 0.180130600929, 0.202233880758, 0.221574291587, 0.240584358573, 0.270628541708, 0.297956854105, 0.339212745428, 0.366749674082, 0.38795030117, 0.410747468472, 0.423748403788, 0.435721606016, 0.447609990835, 0.460197597742, 0.472442746162, 0.484469771385, 0.496513813734, 0.509329855442, 0.522985339165, 0.537288367748, 0.552631735802, 0.568989217281, 0.585000991821, 0.601064920425, 0.61813223362, 0.634984016418, 0.651516616344, 0.668453633785, 0.685909450054, 0.703463852406, 0.720904886723, 0.738381505013, 0.755721747875, 0.772627353668, 0.789677739143, 0.806799650192, 0.823581695557, 0.840311348438, 0.857264339924, 0.874197840691, 0.891162395477, 0.908636808395, 0.926235020161, 0.943802952766, 0.961274623871, 0.978599429131, 0.995316922665, 1.01167237759, 1.02775800228, 1.04373276234, 1.06011426449, 1.07667136192, 1.09313821793, 1.11020362377, 1.12740135193, 1.14521491528, 1.16345453262, 1.18194246292, 1.1997795105, 1.21735739708, 1.23441255093, 1.25137019157, 1.2678040266, 1.28395235538, 1.30012905598, 1.3164845705, 1.33285558224, 1.34985971451, 1.36738979816, 1.3853290081, 1.40322947502, 1.42106461525, 1.43916726112, 1.45689570904, 1.47463846207, 1.49180364609, 1.5087621212, 1.52556335926, 1.54217541218, 1.55880367756, 1.57556414604, 1.592435956, 1.60978937149, 1.62719881535, 1.64503180981, 1.66314613819, 1.68109154701, 1.69913387299, 1.71668934822, 1.73395037651, 1.75102114677, 1.76782631874, 1.78504574299, 1.8026021719, 1.82037055492, 1.83789503574, 1.85568368435, 1.87355351448, 1.89129209518, 1.90905618668, 1.92669677734, 1.94411814213, 1.96138560772, 1.97823297977, 1.99471151829, 2.01089048386, 2.02694320679, 2.04283499718, 2.05865311623, 2.07433128357, 2.08991146088, 2.10544276237, 2.12100625038, 2.1367623806, 2.15247917175, 2.16820430756, 2.18429017067, 2.20045852661, 2.21690797806, 2.23375296593, 2.25142478943, 2.26911425591, 2.28724837303, 2.30542016029, 2.3239634037, 2.34261608124, 2.36058187485, 2.37847566605, 2.3959581852, 2.41374921799, 2.431194067, 2.44894957542, 2.46637511253, 2.48376846313, 2.50152182579, 2.51924037933, 2.53699922562, 2.55417537689, 2.57099199295, 2.58725166321, 2.60355401039, 2.62009501457, 2.63637900352, 2.65270233154, 2.66901493073, 2.6856803894, 2.7026219368, 2.71966028214, 2.73652267456, 2.75370407104, 2.77095389366, 2.78798699379, 2.80478858948, 2.82159805298, 2.83834123611, 2.8550028801, 2.87140130997, 2.88793778419, 2.90444278717, 2.92085528374, 2.93745279312, 2.95416808128, 2.97127270699, 2.98817753792, 3.00502467155, 3.02220726013, 3.03942418098, 3.05650687218, 3.07343769073, 3.0904545784, 3.10712718964, 3.12351560593, 3.14004468918, 3.15660119057, 3.17296004295, 3.18931126595, 3.20583939552, 3.2221326828, 3.23844909668, 3.25474071503, 3.27104568481, 3.28734064102, 3.3036468029, 3.31991147995, 3.33608651161, 3.35239577293, 3.36873483658, 3.38510584831, 3.40152359009, 3.41819953918, 3.43529129028, 3.45258402824, 3.46968007088, 3.48677802086, 3.50413870811, 3.52160954475, 3.53915166855, 3.55668354034, 3.57425045967, 3.59156656265, 3.60857534409, 3.62551164627, 3.64204621315, 3.65889930725, 3.67547345161, 3.69227147102, 3.70885562897, 3.72546601295, 3.74250006676, 3.75916695595, 3.77581238747, 3.79242181778, 3.80902171135, 3.82560062408, 3.84217786789, 3.85876369476, 3.87534856796, 3.89197492599, 3.90856552124, 3.92513990402, 3.94174766541, 3.95834732056, 3.97492170334, 3.99145245552, 4.00797080994, 4.02450656891, 4.04109191895, 4.05750846863, 4.07348918915, 4.08952140808, 4.10548496246, 4.12176465988, 4.13804769516, 4.15431690216, 4.17067193985, 4.18689584732, 4.20288133621, 4.21912908554, 4.23557806015, 4.25195837021, 4.26846265793, 4.28476524353, 4.30151081085, 4.31847476959, 4.33529186249, 4.35200500488, 4.36929035187, 4.38661575317, 4.40372180939, 4.42116260529, 4.43809127808, 4.45519351959, 4.47330474854, 4.4914932251, 4.50902795792, 4.52641916275, 4.54391145706, 4.56138706207, 4.57889080048, 4.59630012512, 4.61379337311, 4.631046772, 4.64830780029, 4.66539287567, 4.68270635605, 4.69971609116, 4.71651411057, 4.73334026337, 4.75034618378, 4.76649332047, 4.78315067291, 4.79934501648, 4.81588602066, 4.83263111115, 4.84962034225, 4.86648750305, 4.88312721252, 4.89976358414, 4.91619110107, 4.93227529526, 4.94892406464, 4.96513175964, 4.98145341873, 4.99739027023, 5.01356840134, 5.03037166595, 5.04717493057, 5.06397819519, 5.08078145981, 5.09758424759, 5.11438751221, 5.13119077682, 5.14799404144, 5.16479730606, 5.18160009384, 5.19840335846, 5.21520662308, 5.2320098877, 5.24881315231, 5.26561594009, 5.28241920471, 5.29922246933, 5.31602573395, 5.33282899857, 5.34963226318, 5.36643505096, 5.38323831558, 5.4000415802, 5.41684484482, 5.43364810944, 5.45045089722, 5.46725416183, 5.48405742645, 5.50086069107, 5.51766395569, 5.53446674347, 5.55127000809, 5.56807327271, 5.58487653732, 5.60167980194, 5.61848258972, 5.63528585434, 5.65208911896, 5.66889238358, 5.68569564819, 5.70249843597, 5.71930170059, 5.73610496521, 5.75290822983, 5.76971149445, 5.78651428223, 5.80331754684, 5.82012081146, 5.83692407608, 5.8537273407, 5.87053060532, 5.8873333931, 5.90413665771, 5.92093992233, 5.93774318695, 5.95454645157, 5.97134923935, 5.98815250397, 6.00495576859, 6.0217590332, 6.03856229782, 6.0553650856, 6.07216835022, 6.08897161484, 6.10577487946, 6.12257814407, 6.13938093185, 6.15618419647, 6.17298746109, 6.18979072571, 6.20659399033, 6.22339677811, 6.24020004272, 6.25700330734, 6.27380657196, 6.29060983658, 6.30741262436, 6.32421588898, 6.34101915359, 6.35782241821, 6.37462568283, 6.39142847061, 6.40823173523, 6.42503499985, 6.44183826447, 6.45864152908, 6.4754447937, 6.49224758148, 6.5090508461, 6.52585411072, 6.54265737534, 6.55946063995, 6.57626342773, 6.59306669235, 6.60986995697, 6.62667322159, 6.64347648621, 6.66027927399, 6.6770825386, 6.69388580322, 6.71068906784, 6.72749233246, 6.74429512024, 6.76109838486, 6.77790164948, 6.79470491409, 6.81150817871, 6.82831096649, 6.84511423111, 6.86191749573, 6.87872076035, 6.89552402496, 6.91232681274, 6.92913007736, 6.94593334198, 6.9627366066, 6.97953987122, 6.996342659, 7.01314592361, 7.02994918823, 7.04675245285, 7.06355571747, 7.08035898209, 7.09716176987, 7.11396503448, 7.1307682991, 7.14757156372, 7.16437482834, 7.18117761612, 7.19798088074, 7.21478414536, 7.23158740997, 7.24839067459, 7.26519346237, 7.28199672699, 7.29879999161, 7.31560325623, 7.33240652084, 7.34920930862, 7.36601257324, 7.38281583786, 7.39961910248, 7.4164223671, 7.43322515488, 7.45002841949, 7.46683168411, 7.48363494873, 7.50043821335, 7.51724100113, 7.53404426575, 7.55084753036, 7.56765079498, 7.5844540596, 7.60125684738, 7.618060112, 7.63486337662, 7.65166664124, 7.66846990585, 7.68527317047, 7.70207595825, 7.71887922287, 7.73568248749, 7.75248575211, 7.76928901672, 7.7860918045, 7.80289506912, 7.81969833374, 7.83650159836, 7.85330486298, 7.87010765076, 7.88691091537, 7.90371417999, 7.92051744461, 7.93732070923, 7.95412349701, 7.97092676163, 7.98773002625, 8.00453281403, 8.02133655548, 8.03813934326, 8.05494308472, 8.0717458725, 8.08854866028, 8.10535240173, 8.12215518951, 8.13895893097, 8.15576171875, 8.17256450653, 8.18936824799, 8.20617103577, 8.22297477722, 8.239777565, 8.25658035278))
)

uncertaintyCurve = cms.PSet(
   Class = cms.string("LinInterpolatedTable1D"),
   xmin = cms.double(0.000102969119325),
   xmax = cms.double(0.015941093673),
   leftExtrapolationLinear = cms.bool(False),
   rightExtrapolationLinear = cms.bool(True),
   data = cms.vdouble((0.0, 0.00505714397877, 0.0102511914447, 0.0204280037433, 0.0292646884918, 0.0413722880185, 0.0575956813991, 0.0819364264607, 0.104071713984, 0.13140809536, 0.177709490061, 0.22501654923, 0.266917437315, 0.297311335802, 0.321571499109, 0.336695879698, 0.342416554689, 0.340442478657, 0.339388728142, 0.333209782839, 0.329781234264, 0.322183400393, 0.312691986561, 0.305082648993, 0.300921589136, 0.298008352518, 0.285286933184, 0.276323586702, 0.253645569086, 0.245030388236, 0.242959231138, 0.239618569613, 0.246120408177, 0.254121899605, 0.262316614389, 0.269627600908, 0.276411116123, 0.283333569765, 0.290538460016, 0.399373471737, 0.363783746958, 0.352354794741, 0.339878797531, 0.336136221886, 0.334686458111, 0.33659529686, 0.336019933224, 0.333157062531, 0.335065603256, 0.335591584444, 0.322829157114, 0.313258230686, 0.316570669413, 0.321495443583, 0.326235473156, 0.331025868654, 0.33562463522, 0.340017169714, 0.344192266464, 0.347707122564, 0.350935667753, 0.353959619999, 0.357348233461, 0.3602219522, 0.362288326025, 0.364270955324, 0.366030007601, 0.367693632841, 0.368822365999, 0.369616836309, 0.370670109987, 0.371356368065, 0.371764779091, 0.372117489576, 0.372670799494, 0.373141795397, 0.373686224222, 0.374554634094, 0.375796318054, 0.377217769623, 0.378525555134, 0.380155831575, 0.381863445044, 0.383712142706, 0.385804623365, 0.387816548347, 0.389782875776, 0.3916439116, 0.393189936876, 0.394423902035, 0.395244538784, 0.395719856024, 0.395975887775, 0.396043628454, 0.395887821913, 0.394387573004, 0.393044918776, 0.391769826412, 0.390606552362, 0.390182703733, 0.391122847795, 0.392969787121, 0.394981741905, 0.397458672523, 0.400075793266, 0.403020560741, 0.405630975962, 0.408069968224, 0.410143345594, 0.411404341459, 0.412260353565, 0.413057714701, 0.412991702557, 0.412594348192, 0.411984115839, 0.411214768887, 0.41081610322, 0.410450309515, 0.410627812147, 0.411173135042, 0.41220381856, 0.413389742374, 0.414489865303, 0.415721148252, 0.416852742434, 0.417886644602, 0.418787539005, 0.419353485107, 0.420055001974, 0.420822799206, 0.421481788158, 0.421926826239, 0.422531932592, 0.423223614693, 0.423797488213, 0.424349248409, 0.424785584211, 0.42518517375, 0.424540311098, 0.42415201664, 0.423572689295, 0.422878861427, 0.422651171684, 0.422811359167, 0.423472523689, 0.424446225166, 0.425987929106, 0.427034884691, 0.428483814001, 0.429750353098, 0.430937588215, 0.431849181652, 0.432482749224, 0.433233439922, 0.434224337339, 0.435773819685, 0.43712490797, 0.438628405333, 0.440533190966, 0.442393451929, 0.443982303143, 0.445604205132, 0.446907311678, 0.447757035494, 0.448184490204, 0.448325842619, 0.447703868151, 0.447088837624, 0.446669697762, 0.446452498436, 0.446226149797, 0.446359068155, 0.446345150471, 0.4475055933, 0.449036031961, 0.450667917728, 0.452159583569, 0.453692704439, 0.455314457417, 0.456971943378, 0.458485275507, 0.459906131029, 0.46104568243, 0.461979836226, 0.46303281188, 0.463504970074, 0.464095026255, 0.464119195938, 0.463767081499, 0.463190555573, 0.462642401457, 0.462019890547, 0.461539238691, 0.461105793715, 0.460867553949, 0.460854917765, 0.460226088762, 0.460445195436, 0.460822999477, 0.461174726486, 0.461308330297, 0.46159645915, 0.462113320827, 0.462802886963, 0.46370446682, 0.464415103197, 0.465312033892, 0.466105222702, 0.467076838017, 0.468101501465, 0.469014078379, 0.469683974981, 0.470458477736, 0.471080034971, 0.471811920404, 0.472737014294, 0.473224937916, 0.473471194506, 0.473653137684, 0.473715662956, 0.473821699619, 0.47403344512, 0.474207669497, 0.474268168211, 0.474437654018, 0.474609553814, 0.474781423807, 0.475044280291, 0.475528210402, 0.475839167833, 0.476361423731, 0.476741403341, 0.476744055748, 0.476911097765, 0.477350711823, 0.477837860584, 0.478302747011, 0.478966414928, 0.479757994413, 0.480286240578, 0.480864673853, 0.481654256582, 0.482313960791, 0.483767271042, 0.485645532608, 0.487122148275, 0.488851815462, 0.490254819393, 0.491659015417, 0.49283388257, 0.493998855352, 0.49549767375, 0.496890217066, 0.498133659363, 0.499097198248, 0.499809622765, 0.500746965408, 0.501603662968, 0.502817809582, 0.503727257252, 0.504615306854, 0.504761815071, 0.505281984806, 0.505869984627, 0.506655812263, 0.507215201855, 0.508225440979, 0.509002387524, 0.509581983089, 0.51035630703, 0.511131823063, 0.511603832245, 0.512016773224, 0.51206189394, 0.511915206909, 0.511912763119, 0.511866390705, 0.512489199638, 0.513209879398, 0.513918936253, 0.514606118202, 0.515344440937, 0.516162157059, 0.51682060957, 0.51762264967, 0.51834911108, 0.519081771374, 0.519984662533, 0.520794451237, 0.521535098553, 0.522220015526, 0.523044288158, 0.523928642273, 0.524731755257, 0.525548815727, 0.526368319988, 0.527004420757, 0.527787685394, 0.528584897518, 0.529246449471, 0.52999740839, 0.530753910542, 0.531503975391, 0.532239675522, 0.532882809639, 0.533571124077, 0.534276008606, 0.534900546074, 0.535448789597, 0.535929560661, 0.536549389362, 0.537192583084, 0.537973642349, 0.538641154766, 0.539255678654, 0.539881885052, 0.54052567482, 0.541155636311, 0.541825294495, 0.542548239231, 0.543206632137, 0.544027149677, 0.544731736183, 0.545692563057, 0.54646718502, 0.547355532646, 0.548060059547, 0.548771440983, 0.549500405788, 0.550197839737, 0.551068603992, 0.552174150944, 0.552947163582, 0.553752601147, 0.554616034031, 0.555410802364, 0.556152701378, 0.556935667992, 0.557715535164, 0.558517575264, 0.55927747488, 0.560037434101, 0.560797393322, 0.561557352543, 0.562317252159, 0.56307721138, 0.563837170601, 0.564597070217, 0.565357029438, 0.566116988659, 0.56687694788, 0.567636847496, 0.568396806717, 0.569156765938, 0.569916725159, 0.570676624775, 0.571436583996, 0.572196543217, 0.572956502438, 0.573716402054, 0.574476361275, 0.575236320496, 0.575996220112, 0.576756179333, 0.577516138554, 0.578276097775, 0.579035997391, 0.579795956612, 0.580555915833, 0.581315875053, 0.58207577467, 0.582835733891, 0.583595693111, 0.584355652332, 0.585115551949, 0.585875511169, 0.58663547039, 0.587395370007, 0.588155329227, 0.588915288448, 0.589675247669, 0.590435147285, 0.591195106506, 0.591955065727, 0.592715024948, 0.593474924564, 0.594234883785, 0.594994843006, 0.595754802227, 0.596514701843, 0.597274661064, 0.598034620285, 0.598794519901, 0.599554479122, 0.600314438343, 0.601074397564, 0.60183429718, 0.602594256401, 0.603354215622, 0.604114174843, 0.604874074459, 0.60563403368, 0.606393992901, 0.607153952122, 0.607913851738, 0.608673810959, 0.60943377018, 0.610193669796, 0.610953629017, 0.611713588238, 0.612473547459, 0.613233447075, 0.613993406296, 0.614753365517, 0.615513324738, 0.616273224354, 0.617033183575, 0.617793142796, 0.618553042412, 0.619313001633, 0.620072960854, 0.620832920074, 0.621592819691, 0.622352778912, 0.623112738132, 0.623872697353, 0.62463259697, 0.62539255619, 0.626152515411, 0.626912474632, 0.627672374249, 0.628432333469, 0.62919229269, 0.629952192307, 0.630712151527, 0.631472110748, 0.632232069969, 0.632991969585, 0.633751928806, 0.634511888027, 0.635271847248, 0.636031746864, 0.636791706085, 0.637551665306, 0.638311624527, 0.639071524143, 0.639831483364, 0.640591442585, 0.641351342201, 0.642111301422, 0.642871260643, 0.643631219864, 0.64439111948, 0.645151078701, 0.645911037922, 0.646670997143, 0.647430896759, 0.64819085598, 0.648950815201, 0.649710774422, 0.650470674038, 0.651230633259, 0.65199059248, 0.652750492096, 0.653510451317, 0.654270410538, 0.655030369759, 0.655790269375, 0.656550228596, 0.657310187817, 0.658070147038, 0.658830046654, 0.659590005875, 0.660349965096, 0.661109864712, 0.661869823933, 0.662629783154, 0.663389742374, 0.664149641991, 0.664909601212, 0.665669560432, 0.666429519653, 0.66718941927, 0.66794937849, 0.668709337711, 0.669469296932, 0.670229196548, 0.670989155769, 0.67174911499, 0.672509014606, 0.673268973827, 0.674028933048, 0.674788892269, 0.675548791885, 0.676308751106, 0.677068710327, 0.677828669548, 0.678588569164, 0.679348528385, 0.680108487606))
)
