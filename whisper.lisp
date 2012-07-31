; Chinese whispers

(defpackage :whisper (:use :cl :chanl))
(in-package :whisper)

(defparameter N 10000)

(defun whisper(left right)
  "create a whsiper thread"
  (pcall #'(lambda () (send left (1+ (recv right))))))

(defun main()
  "Simulate passing message from the right most whisper to
the left most"
    (multiple-value-bind (left-most right-most)
	(loop
	   with left-most = (make-instance 'channel)
	   repeat N
	   for left = left-most then right
	   for right = (make-instance 'channel)
	   do (whisper left right)
	   finally (return (values left-most right)))
      (send right-most 1)
      (print (recv left-most))))
