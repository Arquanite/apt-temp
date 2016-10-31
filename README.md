What is apt-temp?
=================

apt-temp is a simple tool that remembers what packages you have installed,
it also provides very easy way to remove them, this is very useful when you
need to install many build dependencies for a project but project turns out
to be unsatisfying and you have many unnecessary packages. What can you do?

- Browse command history and delete unwanted packages
- Live with them (you have less disk space)
- Or you can use apt-temp to install, and delete them with one command


What apt-temp can do?
---------------------

- It adds every installed (by apt-temp) package name to the list
- User can easily view all packages installed by apt-temp
- User can remove/purge all packages installed by apt-temp in one command
- User can easily test if package is installed/not installed/not purged
- It also recognizes what packages are new and what are old (when user type
    *apt-temp install gcc xorg cmake*, packages what are installed before eg. xorg
    won't be added to the list)
- User can clean system in one command (autoremove, autoclean, purge removed packages)
- User can *approve* packages installed by apt-temp (remove from list but not from system)


Help
----

apt-temp has two kinds of actions: zero-argument and multi-argument. Zero-argument 
action does not take arguments (eg. *list*). Multi-argument action can take one
or more arguments (eg. *install* one or more packages) every action that can take
argument is multi-argument. That means you can *install/remove/purge/approve/test* many
packages at once.


Usage: apt-temp \<action> [package1] [package2]:

- help - show this page
- list - show list of installed temporary packages
- remove \<package> - remove package(s)
- purge \<package> - purge package(s)
- remove-all - remove all temporary packages
- purge-all - purge all temporary packages
- install \<package> - install package(s)
- approve \<package> - remove package(s) from the list
- test \<package> - check state of package(s)
- clean - autoremove, autoclean and purge removed packages


How I can install apt-temp?
---------------------------

1. Clone this repo.
2. cd into apt-temp folder.
3. *make*
4. *sudo make install*
5. You're done!

If you are using zsh and you don't have autocomplete for apt-temp add this lines to .zshrc:

```shell
autoload -U +X compinit && compinit
autoload -U +X bashcompinit && bashcompinit
source /usr/share/bash-completion/completions/apt-temp
```

How I can uninstall apt-temp?
-----------------------------

If you don't like apt-temp you can easily remove it by running *sudo make uninstall* from
directory where you have cloned this repo.
