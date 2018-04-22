T = 2;
$fs = 0.1;
$fa = 2;
difference(){
    translate([0,10,0])cube([13+2*T, 40+T, 15]);
    translate([T,-0.01,-0.01])cube([13,50,4]);
    
    translate([13/2+T,40,0])cylinder(r=1.6,h=100,center=true);
    translate([13/2+T,20,0])cylinder(r=1.6,h=100,center=true);
}