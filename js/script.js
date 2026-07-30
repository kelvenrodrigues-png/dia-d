/* =========================================================
   script.js — JavaScript (roda no navegador)
   Obs.: "Java" e "JavaScript" são linguagens diferentes.
   Este arquivo é JavaScript, a única das duas que funciona
   dentro de uma página web.
   ========================================================= */

document.addEventListener('DOMContentLoaded', function () {

  /* ---- Menu mobile ---- */
  var navToggle = document.getElementById('navToggle');
  var mainNav = document.getElementById('mainNav');

  if (navToggle && mainNav) {
    navToggle.addEventListener('click', function () {
      var isOpen = mainNav.classList.toggle('open');
      navToggle.setAttribute('aria-expanded', isOpen ? 'true' : 'false');
    });
    mainNav.querySelectorAll('a').forEach(function (link) {
      link.addEventListener('click', function () {
        mainNav.classList.remove('open');
        navToggle.setAttribute('aria-expanded', 'false');
      });
    });
  }

  /* ---- Animação de entrada ao rolar a página ---- */
  var revealEls = document.querySelectorAll('.reveal');
  var observer = new IntersectionObserver(function (entries) {
    entries.forEach(function (entry) {
      if (entry.isIntersecting) {
        entry.target.classList.add('in');
        observer.unobserve(entry.target);
      }
    });
  }, { threshold: 0.15 });

  revealEls.forEach(function (el) { observer.observe(el); });

  /* ---- Barra de progresso da linha do tempo ---- */
  var timeline = document.getElementById('timeline');
  var timelineFill = document.getElementById('timelineFill');

  function updateTimelineFill() {
    if (!timeline || !timelineFill) return;

    var rect = timeline.getBoundingClientRect();
    var viewportCenter = window.innerHeight * 0.6;

    // quanto da timeline já "passou" do ponto de referência na tela
    var scrolledPast = viewportCenter - rect.top;
    var ratio = scrolledPast / rect.height;

    // limita entre 0 e 1
    ratio = Math.max(0, Math.min(1, ratio));

    timelineFill.style.height = (ratio * 100) + '%';
  }

  window.addEventListener('scroll', updateTimelineFill, { passive: true });
  window.addEventListener('resize', updateTimelineFill);
  updateTimelineFill();

});