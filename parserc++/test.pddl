; IPC5 Domain: Storage Time
; Authors: Alfonso Gerevini and Alessandro Saetti 

(define (domain Storage-Time)
(:requirements :typing :durative-actions)
(:types hoist surface place area - object)

(:constants
		Letter - size_t

		Black
		Color - color_t

		Front
		Back - side_t

		Some_Feeder_Tray
		Some_Finisher_Tray
		EndCap_Entry-BlackContainer_Exit
		HtmOverBlack_Entry-EndCap_Exit
		Finisher2_Tray - (either size_t color_t side_t)
)

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


