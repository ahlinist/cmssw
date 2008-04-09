#ifndef DECAY_STRUCTS_H
#define DECAY_STRUCTS_H

/* maximum number of muons to generate: */
#define MAXMUON 10

 struct t_decayed_hadron
{
  int index;
  double prob[MAXMUON];
  struct t_decay *decay;
  struct t_decayed_hadron *next;
};

struct t_decay
{
  double prob[MAXMUON];
  int first,last;
  t_decayed_hadron *daughter;
  struct t_decay *next;
};
#endif
