T = 1.6;  // thickness

D = 10; 
C = 8;
W = 18;
R1 = 1.5;
R2 = 3;

$fa = 2;
$fs = 0.1;


module face(hole){
    difference(){
        linear_extrude(height=T)
        intersection(){
            union(){
                square([W,C]);
                square([C,W]);
                translate([C,C])circle(r=W-C);
            }
            square([W,W]);
        }
        
        if (hole)
        translate([D,D,-0.01]) cylinder(r1=R1, r2=R2, h=2);
    }
}

face();
rotate([90,0,0])scale([1,1,-1])face(true);
rotate([0,-90,0])scale([1,1,-1])face(true);