((nil . ((eval . (progn
                   ;; require projectile; the following code will fail
                   ;; without it.
                   (require 'projectile)
                   ;; provide a fake "recent" compilation cmd
                   ;; which will be returned by the function
                   ;; `projectile-compilation-command`
                   (puthash (projectile-project-root)
                            "cd build && make"
                            projectile-compilation-cmd-map))))))

         
