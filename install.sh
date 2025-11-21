mkdir ~/bin
cp C_code_formatter.c ~/bin
echo export PATH="$HOME/bin:$PATH" >> ~/.bashrc
cp style ~/bin
chmod +x ~/bin/style
