






document.addEventListener("DOMContentLoaded", () => {
    const toggle_switches = document.querySelectorAll(".toggle-switch");

    toggle_switches.forEach(item => {

        item.addEventListener("click", (e) => {
            
            item.dispatchEvent(new CustomEvent("toggle", {
                detail: {
                    active: item.classList.toggle("active"),
                },
            }));
            
        });
    });
});


