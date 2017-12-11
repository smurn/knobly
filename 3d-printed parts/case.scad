
front_width = 130;
front_t = 2;
front_height = 340;

pcb_width = 100;
pcb_height = 320;
pcb_t = 2;
pcb_spacing = 16;   // from underside pcb to underside front
modthick = 22+4;
clamp_depth = 50;
below = 100;
wood_t = 10;
thinwood_t = 6;
ontop = front_height - below;
a = asin(clamp_depth / ontop);

echo("ANGLE", a);
echo("CLAMP HEIGHT", below/cos(a));


module side(){
    rotate([90,0,0])
    translate([0,0,-wood_t])
    linear_extrude(height=wood_t)
    polygon(points=[
        [-clamp_depth, 0],  // table, rear
        [-clamp_depth, cos(a)*ontop],   // top rear
        [-clamp_depth+cos(a)*modthick, cos(a)*ontop+sin(a)*modthick],   // top front
        [sin(a)*below+cos(a)*modthick, -cos(a)*below+sin(a)*modthick],  // bottom front
        [sin(a)*below, -cos(a)*below],  // bottom rear
        [0, -below/cos(a)], // bottom clamp
        [0,0] // table corner
    ]);
}

module top(){
    translate([-clamp_depth+cos(a)*modthick, 0, cos(a)*ontop+sin(a)*modthick])
    rotate([0,-a,0])
    translate([-modthick+thinwood_t,wood_t,-thinwood_t])
    cube([modthick-thinwood_t, front_width-2*wood_t, thinwood_t]);
}
module bottom(){
    translate([sin(a)*below+cos(a)*modthick, 0, -cos(a)*below+sin(a)*modthick])
    rotate([0,-a,0])
    translate([-modthick+thinwood_t,wood_t,0])
    cube([modthick-thinwood_t, front_width-2*wood_t, thinwood_t]);
}

module back(){
    rotate([0,-a,0])
    translate([0,wood_t,-below])
    cube([thinwood_t,front_width-2*wood_t, front_height]);
}

module front_panel(){
    color("darkslategray",1)
    translate([cos(a)*modthick,0,sin(a)*modthick])
    rotate([0,-a,0])
    translate([0,0,-below])
    rotate([90,0,90])
    linear_extrude(height=front_t)
    square([front_width,front_height]);
}

module pcb(){
    off = modthick - pcb_spacing;
    color("green",1)
    translate([cos(a)*off,0,sin(a)*off])
    rotate([0,-a,0])
    translate([0,(front_width-pcb_width)/2,-below+(front_height-pcb_height)/2])
    rotate([90,0,90])
    linear_extrude(height=pcb_t)
    square([pcb_width,pcb_height]);
}

module assembly(){
    
    side();
    translate([0, front_width-wood_t])side();
    top();
    bottom();
    back();
    front_panel();
    pcb();
    
}

assembly();