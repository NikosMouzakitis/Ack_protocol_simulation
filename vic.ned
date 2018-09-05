
simple Md
{
    	
    gates:
        input i1;
        input i2;
        output o1;
		output o2;
}

network Vic
{
    submodules:
        a: Md;
        bridge: Md;
        //bridging the messages between [node a] and [node c]
        c: Md;
        
    connections allowunconnected:
        
        a.o2 --> {  delay = 100ms; } --> bridge.i1;
		
		bridge.o1 --> { delay = 100ms; } --> a.i1;
		bridge.o2 --> { delay = 100ms; } --> c.i2;
		
		c.o2 --> { delay = 100ms; } --> bridge.i2;
}
