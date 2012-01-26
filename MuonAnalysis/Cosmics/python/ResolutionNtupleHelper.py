upper = 0
lower = 1
n_track_pos = 2

track_nicks = [
  "Global",
  "StAlone",
  "TkOnly",
  "TPFMS",
  "Picky",
  "DYT",
  "TMR",
  "SigSw",
  "TuneP",
  "StaGlb",
  "StaTko"
]

n_tracks = len(track_nicks)

for i,n in enumerate(track_nicks):
    exec 'tk_%s = %i' % (n.lower(), i)

def make_cut_string(min_pixel_layers=1, min_strip_layers=8, no_csc_allowed=True, max_dxy=None, max_dz=None):
    base_cut = []
    if no_csc_allowed:
        base_cut.append('!hit_csc')
    if min_pixel_layers > 0:
        base_cut += ['pixel_layers[%i][%i] >= %i' % (i,j, min_pixel_layers) for j in xrange(n_track_pos) for i in xrange(n_tracks) if i != tk_stalone]
    if min_strip_layers > 0:
        base_cut += ['strip_layers[%i][%i] >= %i' % (i,j, min_strip_layers) for j in xrange(n_track_pos) for i in xrange(n_tracks) if i != tk_stalone]
    if max_dxy:
        base_cut += ['abs(unprop_dxy[%i][%i]) < %f' % (i,j, max_dxy) for j in xrange(n_track_pos) for i in xrange(n_tracks) if i != tk_stalone]
    if max_dz:
        base_cut += ['abs(unprop_dz[%i][%i]) < %f' % (i,j, max_dz) for j in xrange(n_track_pos) for i in xrange(n_tracks) if i != tk_stalone]
    #pprint(base_cut) ; raise 1
    return ' && '.join(base_cut)

