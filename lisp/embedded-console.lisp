(defpackage "EMBEDDED-CONSOLE"
  (:use "CL" "STR"))

(in-package "EMBEDDED-CONSOLE")

(defvar *string-buffer* ""
  "Holds growing expression")

(defun echo-string (s)
  (coerce (format nil "Recieved input: ~10A" (shorten-string s)) 'base-string))

(defun shorten-string (s)
  (declare (string s))
  (let ((l (length s)))
    (if (> l 10)
        (concat (shorten 7 s) (substring (- l 3) nil s))
        s)))

(defun read-string (s)
  (declare (string s))
  (setq *string-buffer* (concatenate 'string *string-buffer* s))
  (handler-case
      (multiple-value-bind (object pos) (read-from-string *string-buffer*)
        (setq *string-buffer* (subseq *string-buffer* pos))
        (values object (coerce (shorten-string *string-buffer*) 'base-string)))
    ((or reader-error end-of-file) (_)
      (values nil (coerce (shorten-string *string-buffer*) 'base-string)))))

(defun to-display-string (o)
  (coerce (shorten-string (write-to-string o)) 'base-string))
