






document.addEventListener("DOMContentLoaded", () => {
    const toggle_switch = document.querySelectorAll('.toggle-switch');

    toggle_switch.forEach(item => {
        item.addEventListener('click', (e) => {
            
            const circle = item.querySelector('.toggle-switch-inner');

            circle.classList.toggle('active-inner');
        
            if (circle.classList.contains('active-inner')) {
                item.style.background = '#3868a0';
            } else {
                item.style.background = '#ccc';
            }
            
        });
    }) 
})