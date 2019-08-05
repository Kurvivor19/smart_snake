(defun echo-string (s)
  (coerce (format nil "Recieved input: ~10A" s) 'base-string))
