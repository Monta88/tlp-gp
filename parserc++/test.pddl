; IPC5 Domain: Storage Time
; Authors: Alfonso Gerevini and Alessandro Saetti 

(define (domain Storage-Time)
(:requirements :typing :durative-actions)
(:types hoist surface place area - object)

(:predicates (clear ?s - storearea)
	     (in ?x - (either storearea crate) ?p - place)
	     (connected ?a1 ?a2 - area))

(:durative-action skyjet-fly
	:parameters (?s - skyjet ?b - skybase)
	:duration
		(= ?duration (flying-timewindow ?s))
	:timepoints (start end flypoint)
	:timealiases (all [start end[)
	:timeconstraints
		(
		(= (- end start) ?duration)
		(< start flypoint)
		(> end flypoint)
		)
	:condition (over all (ready ?s))
	:effect
		(and
		(over all (reserved-skybase ?b))
		(over [flypoint (+ flypoint (flying-time ?s))] (flying ?s))
		(at end (not (reserved-skybase ?b))))))


