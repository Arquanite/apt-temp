_apt-temp()
{
    local sourcesdir="/etc/apt/sources.list.d"
    local cur prev actions
	
	COMPREPLY=()
	cur="${COMP_WORDS[COMP_CWORD]}"
	prev="${COMP_WORDS[COMP_CWORD-1]}"
    local actions=("help list remove purge remove-all purge-all \
        			install approve test clean")

    if [ $COMP_CWORD -eq 1 ]
    then
    	COMPREPLY=( $(compgen -W "${actions}" -- $cur) )
	elif [ $COMP_CWORD -ge 2 ]
	then
	    case "${COMP_WORDS[1]}" in	
    	  remove|purge|approve)
	        COMPREPLY=( $(compgen -W "$(cat /etc/apt-temp/temporary_packages)" $cur) )
	        ;;
	      
	      install)
	        COMPREPLY=( $( apt-cache --no-generate pkgnames "$cur" 2> /dev/null ) )
	        ;;
	        
		  test)
		    COMPREPLY=( $(compgen -W "$(dpkg --get-selections | awk '{ print $1 }')" $cur) )
			;;
	   	  *)
	    	;;
	    esac
	fi
	
    return 0
} &&
complete -F _apt-temp apt-temp
