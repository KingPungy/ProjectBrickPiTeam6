import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
import datetime
import socket
import fcntl
import struct

def get_ip_address(ifname):
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	return socket.inet_ntoa(fcntl.ioctl(
		s.fileno(),
		0x8915,  # SIOCGIFADDR
		struct.pack('256s', ifname[:15])
	)[20:24])

ip = get_ip_address('wlan0')

msg = MIMEMultipart()
msg['From'] = 'thuisbioscoopvangroepvi2@gmail.com'
password = 'Frikandelenpatat2000[]'
msg['To'] = 'rubenaar@gmail.com,scott.timmermans@gmail.com,sander.boot@student.hu.nl,tobias.vandenhoogen@student.hu.nl,mike.hoogendoorn@student.hu.nl'
msg['Subject'] = 'IP van de Brick Pi'
date = datetime.datetime.now()
msg.attach(MIMEText('Het IP adres is: \'{}\'. Dit bericht is verzonden op {} om {}.'.format(ip, date.strftime('%d-%m-%Y'), date.strftime('%H:%M:%S')), 'plain'))

smtp = smtplib.SMTP('smtp.gmail.com', 587)
smtp.starttls()
smtp.login(msg['From'], password)
smtp.sendmail(msg['From'], msg['To'].split(','), msg.as_string())
smtp.quit()
