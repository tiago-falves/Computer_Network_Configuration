ifconfig eth0 up
ifconfig eth0 172.16.40.254

ifconfig eth1 up
ifconfig eth1 172.16.41.253

echo 1 > /proc/sys/net/ipv4/ip_forward
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts

route add -net default gw 172.16.41.254/24