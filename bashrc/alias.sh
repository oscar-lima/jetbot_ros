alias ..='cd .. && ls' # going back one directory and showing files convenient alias
alias sl='ls' # when typing fast sometimes ls gets typed as sl
alias remove_endline_spaces="sed -i 's/\s*$//'" # remove automatically spaces at the end of files, needs the file as argument at the end, i.e. remove_spaces my_file.txt
alias cb='catkin build'
alias cbt='catkin build --this'
