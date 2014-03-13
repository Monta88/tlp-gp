; IPC5 Domain: Storage Time
; Authors: Alfonso Gerevini and Alessandro Saetti 

(define (domain Storage-Time)
(:requirements :typing :durative-actions)
(:types hoist surface place area - object)

(:predicates (clear ?s - storearea)
	     (in ?x - (either storearea crate) ?p - place)
	     (connected ?a1 ?a2 - area))

(:durative-action go-in 
 :parameters (?h - hoist ?from - transitarea ?to - storearea)
 :duration (= ?duration 1)
 :condition (and (at start (at ?h ?from)) (at start (clear ?to))
		 (over all (connected ?from ?to)))
 :effect (and (at start (not (at ?h ?from))) (at start (not (clear ?to)))
	      (at end (at ?h ?to))))
)


