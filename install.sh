mkdir ~/bin
mv C_code_formatter.c ~/bin
echo export PATH="$HOME/bin:$PATH" >> ~/.bashrc
mv style ~/bin
sudo chmod +x ~/bin/style
