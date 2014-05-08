;; helloworld by Alan BENIER

(define (domain as-simple-as-hello)
    (:requirements :typing :durative-actions :numeric-fluents)
    (:types word - object
    		person - object)
    (:constants hello - word
    			world - person)
    (:predicates
        (say ?w - word ?p - person)
    )

    (:durative-action say-hello
     :parameters (?w - word ?p - person)
     :duration (= ?duration 1)
     :condition ()
     :effect (at end (say ?w ?p)
             )
    )
)
