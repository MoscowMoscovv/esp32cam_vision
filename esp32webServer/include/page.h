const char HTML[] = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>
        Mousebot
    </title>
    <meta name="viewport" content="user-scalable=no">
</head>
<body  scroll="no" style="position: fixed; font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif ;
color:rgb(128, 128, 128);
font-size: xx-large;">
    <canvas id="canvas" name="game"></canvas>
    <img id="stream" src="http://192.168.1.2/stream" 
    style="position: absolute; top: 0; left: 0; width: 100%; height: 100%; object-fit: contain; z-index: -1;" 
    alt="Camera Stream">
  <script>
        var canvas, ctx;

        window.addEventListener('load', () => {

            canvas = document.getElementById('canvas');
            ctx = canvas.getContext('2d');          
            resize(); 

            document.addEventListener('mousedown', startDrawing);
            document.addEventListener('mouseup', stopDrawing);
            document.addEventListener('mousemove', Draw);

            document.addEventListener('touchstart', startDrawing, { passive: false });
            document.addEventListener('touchend', stopDrawing, { passive: false });
            document.addEventListener('touchcancel', stopDrawing, { passive: false });
            document.addEventListener('touchmove', Draw, { passive: false });
            window.addEventListener('resize', resize);
            // for(id =0;id<= center_coords.length;id++){
            //     document.getElementById("x_coordinate_"+id).innerText = 0;
            //     document.getElementById("y_coordinate_"+id).innerText = 0;
            //     document.getElementById("speed_"+id).innerText = 0;
            //     document.getElementById("angle_"+id).innerText = 0;
            // }
        });

        
        var center_coords =[{x : width/5, y : height/3}, {x : 4*width/5, y : height/3}]
        var width, height, radius, x_orig, y_orig;

        map_req = new Map();
        for (id = 0; id <= center_coords.length;id++){
            map_req.set('angle'+id,0);
            map_req.set('speed'+id,0);
        }

        function resize() {
            width = window.innerWidth;
            height = window.innerHeight;
            radius = 100;

            // Обновляем расположение джойстиков внизу
            center_coords = [
                { x: (width / 6) +80, y: height - radius -80 },  // первый джойстик
                { x: (width * 5 / 6) -80, y: height - radius - 80 }  // второй джойстик
            ];

            // Устанавливаем размер канваса
            ctx.canvas.width = width;
            ctx.canvas.height = height;

            // Рисуем фон и джойстики
            background();
            joystick(center_coords[0].x, center_coords[0].y, 0);
            joystick(center_coords[1].x, center_coords[1].y, 1);
    }



        function background() {
            for(i=0;i<center_coords.length;i++){
                x_orig = center_coords[i].x;
                y_orig = center_coords[i].y;
                ctx.beginPath();
                ctx.arc(x_orig, y_orig, radius + 20, 0, Math.PI * 2, true);
                ctx.fillStyle = 'rgba(236, 229, 229, 0.1)';
                ctx.fill();
            }
        }

        function draw_circle(x, y) {
            ctx.beginPath();
            ctx.arc(x, y, radius, 0, Math.PI * 2, true);
            ctx.fillStyle = 'rgba(240, 128, 128, 0.2)';  // Полупрозрачный цвет
            ctx.fill();
            ctx.strokeStyle = '#F6ABAB';
            ctx.lineWidth = 8;
            ctx.stroke();
        }

        function is_it_in_the_circle(id,x,y) {
            //console.log(id);
            var current_radius = Math.sqrt(Math.pow(x - center_coords[id].x, 2) + Math.pow(y - center_coords[id].y, 2));
            if (radius >= current_radius) {
                return true;
            }
            else {
                return false;
        }}

        function joystick(x, y, id) {
    x_orig = center_coords[id].x;
    y_orig = center_coords[id].y;
    
    let dx = x - x_orig;
    let dy = y - y_orig;
    let distance = Math.sqrt(dx * dx + dy * dy);
    
    if (distance > radius) {
        x = x_orig + (dx / distance) * radius;
        y = y_orig + (dy / distance) * radius;
    }

    draw_circle(x, y);

    let speed = Math.round(100 * distance / radius);
    let angle = Math.atan2(dy, dx);
    let angle_in_degrees = Math.round(angle < 0 ? -angle * 180 / Math.PI : 360 - angle * 180 / Math.PI);

    if (speed > 100) speed = 100;


    map_req.set('speed'+id,speed);
    map_req.set('angle'+id,angle_in_degrees);
    //document.getElementById(`x_coordinate_${id}`).innerText = Math.round(dx);
    //document.getElementById(`y_coordinate_${id}`).innerText = Math.round(dy);
    //document.getElementById(`speed_${id}`).innerText = speed;
    //document.getElementById(`angle_${id}`).innerText = angle_in_degrees;
}

        let coord = { x: 0, y: 0 };
        let paint = false

        function getPosition(event) {
            var mouse_x = event.clientX || event.touches[0].clientX;
            var mouse_y = event.clientY || event.touches[0].clientY;
            coord.x = mouse_x - canvas.offsetLeft;
            coord.y = mouse_y - canvas.offsetTop;
                
        }

        


        function closest_to_mouse_joystick_id(){
    
            if (Math.sqrt(Math.pow(coord.x - center_coords[0].x, 2) + Math.pow(coord.y - center_coords[0].y, 2)) < Math.sqrt(Math.pow(coord.x - center_coords[1].x, 2) + Math.pow(coord.y - center_coords[1].y, 2))){
                return 0;
            }
            else{
                return 1;
            }
        }

        var start_joystick_id;

        function send_req(){
            request = new XMLHttpRequest();
            s=''
            for(id=0; id<center_coords.length; id++){
                
                if (id!=0){
                    s+='&'
                }
                s+=(id+'speed='+map_req.get('speed'+id)+
                '&'+id+'angle='+map_req.get('angle'+id));
        
            }
            //console.log(s);
            request.open('GET','/joystic_pos?'+s);
            request.send();
        
        }

        function startDrawing(event) {
            paint = true;
            getPosition(event);
            //todo: insert here check for start joystick_id
            start_joystick_id = closest_to_mouse_joystick_id();
            not_start_joystick_id = Number(!start_joystick_id);
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            background();
            if (start_joystick_id==0){
                joystick(coord.x, coord.y, 0);
                joystick(center_coords[1].x, center_coords[1].y,1);
            }
            else{
                joystick(center_coords[0].x,center_coords[0].y,0);
                joystick(coord.x,center_coords[1].y,1);
            }
            
            //Draw();
            send_req();
        }


        function stopDrawing() {
            paint = false;
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            background();
            console.log(center_coords[0].x, center_coords[0].y,center_coords[1].x, center_coords[1].y,1);
            joystick(center_coords[0].x, center_coords[0].y,0);
            joystick(center_coords[1].x, center_coords[1].y,1);
            send_req();
            // for (id in [0,1]){
            //     document.getElementById(`x_coordinate_${id}`).innerText = 0;
            //     document.getElementById(`y_coordinate_${id}`).innerText = 0;
            //     document.getElementById(`speed_${id}`).innerText = 0;
            //     document.getElementById(`angle_${id}`).innerText = 0;
            // }
        }

        function Draw(event) {
            if (!paint) return;
            //start_joystick_id = closest_to_mouse_joystick_id();
            event.preventDefault(); // Предотвращаем прокрутку страницы при мультитаче
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            background();
            getPosition(event);
            if (start_joystick_id==0){
                joystick(coord.x, coord.y, 0);
                joystick(center_coords[1].x, center_coords[1].y,1);
            }
            else{
                joystick(center_coords[0].x,center_coords[0].y,0);
                joystick(coord.x,center_coords[1].y,1);
            }

            send_req();
            }

            
        
    </script>
</body>
</html><br>
)=====";