var dragNum = 1;
var audio = new Audio('ao.mp3');

function allowDrop(ev) {
    ev.preventDefault();
}

function drag(ev) {
    ev.dataTransfer.setData("text", ev.target.id);
}

function drop(ev) {
    ev.preventDefault();
    var data = ev.dataTransfer.getData("text");
    ev.target.appendChild(document.getElementById(data));
    dragNum += 1;
    audio.play();
    dragonBox = document.getElementById("marq2");
    currentHTML = dragonBox.innerHTML;
    dragonBox.innerHTML = currentHTML + '<img class="drag1" id="drag'+dragNum + '" src="http://thumbs.dreamstime.com/x/green-dragon-cartoon-illustration-11670789.jpg" draggable="true" ondragstart="drag(event)">'

}
