.pragma library
// Функция форматирования времени (00:00)
function formatTime(s) {
    let m = Math.floor(s / 60);
    let sec = s % 60;
    return (m < 10 ? "0" : "") + m + ":" + (sec < 10 ? "0" : "") + sec;
}
