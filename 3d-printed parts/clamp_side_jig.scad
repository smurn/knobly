T = 2;
$fs = 0.1;
$fa = 2;

intersection(){
    union(){
        difference(){
            translate([0,00,0])cube([13+2*T, 60+2*T, 15]);
            translate([T,T,-0.01])cube([13,60,4]);
            
            translate([13/2+T,15+T,0])cylinder(r=1.6,h=100,center=true);
            translate([13/2+T,45+T,0])cylinder(r=1.6,h=100,center=true);
        }

        translate([0,0,-10])cube([13+2*T,T,10]);
        translate([0,60+T,-10])cube([13+2*T,T,10]);
    }
    translate([0,10,-10])cube(100);
}