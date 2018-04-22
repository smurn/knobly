$fa = 2;
$fs = 0.1;

difference(){
    cube([16, 40, 25]);
    translate([1.8,-1,-0.01])cube([12.4,100,4]);
    translate([8,5,-0.01])cylinder(r=2.5+0.1,h=100,center=true);
    
    translate([8,15+20,20-20])cube([1.6, 40, 40], center=true);
}