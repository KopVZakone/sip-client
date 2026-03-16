.pragma library
// Функция форматирования времени (00:00)
function formatTime(s) {
    let m = Math.floor(s / 60);
    let sec = s % 60;
    return (m < 10 ? "0" : "") + m + ":" + (sec < 10 ? "0" : "") + sec;
}
// Функция форматирования даты звонка
function formatTimestamp(isoString) {
    let date = new Date(isoString);
    let now = new Date();

    let isToday = date.toDateString() === now.toDateString();
    let yesterday = new Date();
    yesterday.setDate(now.getDate() - 1);
    let isYesterday = date.toDateString() === yesterday.toDateString();

    let timeStr = date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });

    if (isToday)
        return "Сегодня, " + timeStr;
    if (isYesterday)
        return "Вчера, " + timeStr;
    return date.toLocaleDateString() + " " + timeStr;
}
