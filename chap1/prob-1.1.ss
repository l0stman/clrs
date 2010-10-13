#lang scheme
(define second 1)
(define minute (* 60 second))
(define hour (* 60 minute))
(define day (* 24 hour))
(define month (* 30 day))
(define year (* 365 month))
(define century (* 100 year))

(define param (list second minute hour day month year century))

(define (maxsize f init step time)
  (let ((delta 1e-6))
    (define (iter step n)
      (if (> (* (* (f n)) 
                delta)
             time)
          n
          (iter step (+ step n))))
    (define (iter-step init step)
      (let ((res (iter step init)))
        (if (= step 1)
            res
            (iter-step (- res step) (/ step 10)))))
    (iter-step init step)))