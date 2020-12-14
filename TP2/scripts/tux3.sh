ifconfig eth0 up
ifconfig eth0 172.16.40.1/24

route add -net 172.16.41.0/24 gw 172.16.40.254

route add -net default gw 172.16.40.254