apt-temp:
	g++ -o apt-temp -std=c++11 apt-temp.cpp

install: apt-temp
	mkdir /etc/apt-temp
	touch /etc/apt-temp/temporary_packages
	cp bash_completion.sh /usr/share/bash-completion/completions/apt-temp
	cp apt-temp /usr/bin/

zsh-completion:
	

clean:
	rm apt-temp

uninstall:
	rm /usr/share/bash-completion/completions/apt-temp
	rm /etc/apt-temp/temporary_packages
	rmdir /etc/apt-temp
	rm /usr/bin/apt-temp
