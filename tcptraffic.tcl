set ns [new Simulator]

set nf [open out.nam w]

$ns namtrace-all $nf

proc finish {} {

        global ns nf

        $ns flush-trace

        close $nf

        exec nam out.nam &

        exit 0

}

set n0 [$ns node]

set n1 [$ns node]

set n2 [$ns node]

set n3 [$ns node]

 

$ns duplex-link $n0 $n1 1Mb 10ms DropTail

$ns duplex-link $n0 $n2 1Mb 10ms DropTail

$ns duplex-link $n0 $n3 1Mb 10ms DropTail

 

#create a tcp connection and attach to node n1

set tcp1 [new Agent/TCP]

$ns attach-agent $n1 $tcp1

$tcp1 set window_ 8

$tcp1 set fid_ 1

 

#create a tcp connection and attach to node n2

set tcp2 [new Agent/TCP]

$ns attach-agent $n2 $tcp2

$tcp2 set window_ 8

$tcp2 set fid_ 2

 

#create the sink nodes 1 and 2

set sink1 [new Agent/TCPSink]

set sink2 [new Agent/TCPSink]

 

#attach sink 1 and 2 to node n3

$ns attach-agent $n3 $sink1

$ns attach-agent $n3 $sink2

 

$ns connect $tcp1 $sink1

$ns connect $tcp2 $sink2

 

#Create FTP applications and attach them to agents

set ftp1 [new Application/FTP]

$ftp1 attach-agent $tcp1

set ftp2 [new Application/FTP]

$ftp2 attach-agent $tcp2

 

$ns at 0.1 "$ftp1 start"

$ns at 0.5 "$ftp2 start"

$ns at 3.5 "$ftp1 stop"

$ns at 5.0 "$ftp2 stop"

 

$ftp1 set class_ 1

$ftp2 set class_ 2

 

$ns color 1 Red 

$ns color 2 Blue

 

$ns at 5.0 "finish"

$ns run