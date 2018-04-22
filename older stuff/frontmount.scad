
PCBFRONT = 14.4;
NOZZLE = 0.39;
T = 2;
$fa = 2;
$fs = 0.2;
CONN_W = 5;

module mount(){
    scale([1,1,-1])
    difference(){
        union(){
            cylinder(d=6,h=PCBFRONT);
            cylinder(r=7,h=T);
            
            rotate([90,0,0])
            linear_extrude(height=6*NOZZLE, center=true)
            polygon(points=[
                [0,T],
                [6.9,T],
                [2.8,PCBFRONT],
                [-2.8,PCBFRONT],
                [-6.9,T]
            ]);
        }
        translate([0,0,T])cylinder(d=6-4*NOZZLE,h=100);
    }
}

module connector(from, to){
    dx = to[0]-from[0];
    dy = to[1]-from[1];
    length = sqrt(dx*dx + dy*dy);
    angle = atan2(dy, dx);
    
    translate(from)
    rotate(angle)
    scale([1,1,-1])
    linear_extrude(height=1)
    difference(){
        translate([0,-CONN_W/2]) square([length, CONN_W]);
        circle(d=9);
        translate([length,0])circle(d=9);
    }
}

module guide(d=8){
    scale(-1)
    linear_extrude(height=1)
    difference(){
        circle(d=12);
        circle(d=d);
    }
}

module bottom(){
    translate([203,-336.5])rotate(90)mount();
    translate([161,-336.5])rotate(90)mount();
    translate([130,-342])rotate(90)mount();
    translate([129,-308])rotate(90)mount();
    translate([161,-290])rotate(90)mount();
    translate([202,-265])mount();
    
    translate([144,-284])guide();
    translate([208,-252])guide();
    translate([176,-284])guide();
    translate([196,-327])guide(7);
    translate([140,-346])guide(7);
    
    connector([203,-336.5], [161,-336.5]);
    connector([161,-336.5], [130,-342+2]);
    connector([130,-342], [129,-308]);
    connector([129,-308+2], [161,-290]);
    //connector([161,-290], [202,-265]);
    
    connector([176,-284], [202,-265]);
    connector([161,-290], [176,-284]);
    
    connector([202-0.5,-265], [203+0.5,-336.5]);
    
    connector([161,-290], [144,-284]);
    connector([202,-265], [208,-252]);
    
    //connector([196,-327], [161,-290]);
}

module middle(){
    
    translate([111,-233])rotate(90)mount();
    translate([159,-233])rotate(90)mount();
    translate([207,-233])rotate(90)mount();
    
    translate([111,-196])rotate(90)mount();
    translate([159,-196])rotate(90)mount();
    translate([207,-196])rotate(90)mount();
    
    translate([111,-154])rotate(90)mount();
    translate([159,-154])rotate(90)mount();
    translate([207,-154])rotate(90)mount();
 
    translate([205,-216])guide();
    translate([112,-171])guide();
    translate([208,-171])guide();
    translate([144,-252])guide();
    
    
    connector([111,-233], [159,-233]);
    connector([207,-233], [159,-233]);
    
    connector([111,-154],[159,-154]);
    connector([207,-154],[159,-154]);
    
    connector([207,-233],[205,-216]);
    connector([207,-196],[205,-216]);
    connector([207,-196],[208,-171]);
    connector([207,-154],[208,-171]);
    
    connector([111,-154], [112,-171]);
    //connector([111,-196], [112,-171]);
    //connector([111,-196], [111,-233]);
    
    connector([144,-252], [159,-233]);
    
    connector([159,-196],[207,-196]);
    connector([159,-196],[111,-196]);
    //connector([159,-196],[159,-233]);
    connector([159-8.5,-196-5],[159-8.5,-154+5]);
}

module top(){
    translate([126,-103])mount();
    translate([207,-111])mount();
    translate([111,-71])mount();
    translate([207,-54])mount();
 
    translate([141,-90])guide();
    translate([205,-90])guide();
    
    connector([126,-103],[207,-111]);
    //connector([111,-71],[207,-54]);
    connector([111,-71],[141,-90]);
    connector([111,-71],[126,-103]);
    
    connector([126,-103],[141,-90]);
    
    connector([207,-111],[205,-90]);
    connector([207,-54],[205,-90]);
    
    connector([141,-90],[207,-54]);
    
}


//top();
//middle();
bottom();