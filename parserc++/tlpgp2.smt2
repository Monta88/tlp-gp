(set-option :produce-models true)
(declare-fun GoalsE0 () Bool )
(declare-fun t_GoalsE0 () Int )
(declare-fun InitsE1 () Bool )
(declare-fun t_InitsE1 () Int )
(assert (and GoalsE0
InitsE1
(< t_InitsE1 1 )
))
(check-sat)
(get-value (GoalsE0
t_GoalsE0
InitsE1
t_InitsE1
 ) )
(exit)
