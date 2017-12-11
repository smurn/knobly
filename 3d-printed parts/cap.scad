
L = 9.8;
W = 4.8;
H = 13;
R = 1;
$fa = 2;
$fs = 0.1;
PW = 2.95;
N = 0.39;   // nozzle
X = 1;
lock = 0.4;
tamper = 0.4;
/*****

Bed: 0 C°
Temp: 190 C°
Nozzle: 0.4 mm
Speed: 40 mm/s
Layer: 0.1 mm
Materialflow 105%
Retraction: all
Infill: 0%
Top/Bottom: 2 mm

We want 100% infill on the last 2 mm, and 0% below.
We can get this by setting infill to 0% and use
the top/bottom thing to create the infill. The geometry
does allow for infill in the lowest 2 mm, so the bottom
infill does not affect us.


*****/

module outside_profile(){
    square([L-2*R,W], center=true);
    square([L,W-2*R], center=true);
    translate([(L/2-R), (W/2-R)]) circle(r=R);
    translate([-(L/2-R), (W/2-R)]) circle(r=R);
    translate([(L/2-R), -(W/2-R)]) circle(r=R);
    translate([-(L/2-R), -(W/2-R)]) circle(r=R);
}

module outside(){
    linear_extrude(height=H-tamper ){ // X+1.9
        outside_profile();
    }
}

module tamper(){
    translate([0,+W/2,H])rotate([45,0,0])cube([20,2*tamper,2*tamper],center=true);
    translate([0,-W/2,H])rotate([45,0,0])cube([20,2*tamper,2*tamper],center=true);
    
    translate([+L/2,0,H])rotate([0,45,0])cube([2*tamper,20,2*tamper],center=true);
    translate([-L/2,0,H])rotate([0,45,0])cube([2*tamper,20,2*tamper],center=true);
}



module inside(){
    
    module cutout(){
        translate([0,-PW/2,X])
        rotate([90,0,90])
        linear_extrude(height=PW+0.2, center=true)
        polygon(points=[
            [-0.1,-1],
            [lock,-1],
            [lock,-0.5],
            [lock,0],
            [lock,0],
            [-0.1,0]
        ]);
    }
    
    module side(){
        difference(){
            union(){
                l = L - 4*N;
                w = W - 4*N;
                r = R - 2*N;
                square([l-2*r,w], center=true);
                square([l,w-2*r], center=true);
                translate([(L/2-R), (W/2-R)]) circle(r=r);
                translate([-(L/2-R), (W/2-R)]) circle(r=r);
                translate([(L/2-R), -(W/2-R)]) circle(r=r);
                translate([-(L/2-R), -(W/2-R)]) circle(r=r);
            }
            square([PW+4*N,100], center=true);
        }
    }
    
    
    
    difference(){
        linear_extrude(height=H-2, convexity=4){
            square([PW, PW], center=true);
            //translate([PW/2 + 2*N, -W/2+2*N]) square([L/2-PW/2-4*N, W-4*N]);
            //scale([-1,1,1])translate([PW/2 + 2*N, -W/2+2*N]) square([L/2-PW/2-4*N, W-4*N]);
            square([L-4*N, PW-4*N], center=true);
            side();
        }
        cutout();
        scale([1,-1,1])cutout();
    }
    
    

}

module roundtop(){
    module corner(){
        translate([-W/2,-W/2,0])
        linear_extrude(height=tamper, scale=(W-2*tamper)/W )
        intersection(){
            translate([-L/2+W/2,0,0])outside_profile();
            square(100,100);
        }
    }
    
    scale([ 1, 1, 1])translate([L/2, W/2])corner();
    scale([ 1,-1, 1])translate([L/2, W/2])corner();
    scale([-1, 1, 1])translate([L/2, W/2])corner();
    scale([-1,-1, 1])translate([L/2, W/2])corner();
    
    linear_extrude(height=tamper, scale=(W-2*tamper)/W )
    translate([0,0,0])square([L-2*R,W], center=true);
}

module backcut(){
    // these are to ensure a consistant wall thickness to ensure
    // that the cirtical parts can be printed without infill.
    module cut(from_z, to_z, depth){
        translate([-PW/2,PW/2+2*N-depth,from_z])
        cube([PW,10,to_z-from_z]);
    }
    
    cut(-0.01,1,lock);
    cut(1-0.01,5,0);
}
difference(){
    union(){
        translate([0,0,H-tamper])roundtop();
        outside(); 
    }
    translate([0,0,-0.01])inside();
    backcut();
    scale([1,-1,1])backcut();
    
    //tamper();
}

