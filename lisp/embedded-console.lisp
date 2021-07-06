(defpackage "EMBEDDED-CONSOLE"
  (:use "CL" "STR"))

(in-package "EMBEDDED-CONSOLE")

(defun echo-string (s)
  (coerce (format nil "Recieved input: ~10A" (shorten-string s)) 'base-string))

(defun shorten-string (s)
  (declare (string s))
  (let ((l (length s)))
    (if (> l 10)
        (concat (shorten 7 s) (substring (- l 3) nil s))
        s)))
