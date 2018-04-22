
$fa = 4;
$fs = 0.2;

module drill(){
    cylinder(d=3.2,h=10,center=true);
    translate([0,0,0.5])cylinder(d1=6,d2=3.2,h=1.8);
    translate([0,0,-0.01])cylinder(d=6,h=0.52);
}

difference(){
    cube([40,12,3]);
    
    translate([10,6,0]) drill();
    translate([30,6,0]) drill();
}