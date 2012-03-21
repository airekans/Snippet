(defun last-blank-block ()
 (interactive)
 (search-backward-regexp "^[ \t]*[^ \t\n]+.*\n\\([ \t]*\n\\)+"
  (point-min)
  1)
 (next-line))

(defun next-blank-block ()
 (interactive)
 (search-forward-regexp "\\(\n[ \t]*\\)+\n[ \t]*[^ \t\n]+"
  (point-max)
  1)
 (move-beginning-of-line 0))

(global-set-key (kbd "C-{") 'last-blank-block)
(global-set-key (kbd "C-}") 'next-blank-block)
