ifconfig eth0 up
ifconfig eth0 172.16.41.1/24

route add -net 172.16.40.0/24 gw 172.16.41.253

route add -net default gw 172.16.41.254