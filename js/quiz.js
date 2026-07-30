/* =========================================================
   quiz.js — Quiz interativo sobre Mary W. Jackson
   Roda no navegador (JavaScript), usando os elementos
   já existentes no HTML dentro de <section id="quiz">.
   As perguntas são as mesmas do programa em C, adaptadas
   para rodar direto na página.
   ========================================================= */

document.addEventListener('DOMContentLoaded', function () {

  var banco_perguntas = [
    {
      pergunta: "Em que ano Mary Jackson nasceu?",
      opcoes: ["1911", "1921", "1931", "1941"],
      correta: 1,
      explicacao: "Ela nasceu em 9 de abril de 1921, em Hampton, Virgínia."
    },
    {
      pergunta: "Em que instituição ela se formou em 1942?",
      opcoes: ["Hampton Institute", "MIT", "Universidade de Virginia", "Howard University"],
      correta: 0,
      explicacao: "Formou-se com honras máximas no Hampton Institute, em Matemática e Ciências Físicas."
    },
    {
      pergunta: "Em que ano ela entrou para o NACA (futura NASA)?",
      opcoes: ["1945", "1951", "1958", "1962"],
      correta: 1,
      explicacao: "Ela começou em 1951, na unidade segregada West Area Computing."
    },
    {
      pergunta: "Qual era o nome da unidade onde ela começou a trabalhar?",
      opcoes: ["Mission Control", "West Area Computing", "Flight Dynamics Lab", "Propulsion Division"],
      correta: 1,
      explicacao: "West Area Computing era o setor reservado a matemáticas negras, chefiado por Dorothy Vaughan."
    },
    {
      pergunta: "Quem foi o engenheiro que a convidou para trabalhar no túnel de vento?",
      opcoes: ["Werner von Braun", "Kazimierz Czarnecki", "Christopher Kraft", "John Glenn"],
      correta: 1,
      explicacao: "Kazimierz Czarnecki sugeriu que ela se tornasse engenheira."
    },
    {
      pergunta: "O que ela precisou fazer para poder cursar as disciplinas exigidas?",
      opcoes: ["Mudar de cidade", "Pedir autorização especial à prefeitura de Hampton", "Pagar uma multa", "Prestar um novo vestibular"],
      correta: 1,
      explicacao: "Ela teve que pedir permissão para assistir aulas numa escola ainda segregada."
    },
    {
      pergunta: "Em que ano ela se tornou a primeira engenheira negra da NASA?",
      opcoes: ["1948", "1958", "1968", "1978"],
      correta: 1,
      explicacao: "A promoção aconteceu em 1958."
    },
    {
      pergunta: "Em 1979, que cargo ela aceitou (com redução de nível) para ajudar outras pessoas?",
      opcoes: ["Diretora de voo", "Gerente do Programa Federal da Mulher", "Piloto de testes", "Chefe de imprensa"],
      correta: 1,
      explicacao: "Ela passou a recrutar e promover mulheres e minorias dentro da NASA."
    },
    {
      pergunta: "Depois de quantos anos de carreira ela se aposentou, em 1985?",
      opcoes: ["20 anos", "27 anos", "34 anos", "40 anos"],
      correta: 2,
      explicacao: "Foram 34 anos de carreira dedicados à NASA."
    },
    {
      pergunta: "Em que ano a sede da NASA em Washington foi renomeada em homenagem a ela?",
      opcoes: ["2005", "2016", "2021", "2024"],
      correta: 2,
      explicacao: "Em fevereiro de 2021, o prédio passou a se chamar Mary W. Jackson NASA Headquarters."
    }
  ];

  /* ---- elementos da página ---- */
  var quizStart      = document.getElementById('quizStart');
  var quizStartBtn    = document.getElementById('quizStartBtn');
  var quizQuestion    = document.getElementById('quizQuestion');
  var quizResult      = document.getElementById('quizResult');

  var progressFill    = document.getElementById('quizProgressFill');
  var progressLabel   = document.getElementById('quizProgressLabel');
  var questionText    = document.getElementById('quizQuestionText');
  var optionsBox      = document.getElementById('quizOptions');
  var explanationBox  = document.getElementById('quizExplanation');
  var nextBtn          = document.getElementById('quizNextBtn');

  var scoreLabel        = document.getElementById('quizScoreLabel');
  var resultTitle     = document.getElementById('quizResultTitle');
  var resultText       = document.getElementById('quizResultText');
  var retryBtn          = document.getElementById('quizRetryBtn');
  var ringFill           = document.getElementById('quizRingFill');
  var ringLabel          = document.getElementById('quizRingLabel');

  var LETRAS = ['A', 'B', 'C', 'D'];
  var RING_PERIMETRO = 2 * Math.PI * 52; // raio 52, mesmo valor do SVG no HTML

  if (!quizStart || !quizStartBtn) return; // seção de quiz não está na página

  var perguntasRodada = [];
  var indiceAtual = 0;
  var acertos = 0;

  /* embaralha o array (Fisher-Yates), igual ao programa em C */
  function embaralhar(lista) {
    for (var i = lista.length - 1; i > 0; i--) {
      var j = Math.floor(Math.random() * (i + 1));
      var temp = lista[i];
      lista[i] = lista[j];
      lista[j] = temp;
    }
    return lista;
  }

  function mostrarPainel(painel) {
    [quizStart, quizQuestion, quizResult].forEach(function (p) {
      p.hidden = (p !== painel);
    });
  }

  function iniciarQuiz() {
    perguntasRodada = embaralhar(banco_perguntas.slice());
    indiceAtual = 0;
    acertos = 0;
    mostrarPainel(quizQuestion);
    mostrarPergunta();
  }

  function mostrarPergunta() {
    var pergunta = perguntasRodada[indiceAtual];

    progressFill.style.width = ((indiceAtual / perguntasRodada.length) * 100) + '%';
    progressLabel.textContent = 'Pergunta ' + (indiceAtual + 1) + ' de ' + perguntasRodada.length;
    scoreLabel.textContent = acertos + ' acerto' + (acertos === 1 ? '' : 's');

    questionText.textContent = pergunta.pergunta;
    explanationBox.hidden = true;
    explanationBox.textContent = '';
    nextBtn.hidden = true;

    optionsBox.innerHTML = '';
    pergunta.opcoes.forEach(function (opcao, i) {
      var btn = document.createElement('button');
      btn.className = 'quiz-option';
      btn.type = 'button';
      btn.setAttribute('data-letter', LETRAS[i]);
      btn.textContent = opcao;
      btn.addEventListener('click', function () {
        responder(i, btn);
      });
      optionsBox.appendChild(btn);
    });
  }

  function responder(indiceEscolhido, botaoClicado) {
    var pergunta = perguntasRodada[indiceAtual];
    var opcoesEls = optionsBox.querySelectorAll('.quiz-option');

    opcoesEls.forEach(function (el, i) {
      el.disabled = true;
      if (i === pergunta.correta) {
        el.classList.add('correct');
      } else if (i === indiceEscolhido) {
        el.classList.add('incorrect');
      }
    });

    if (indiceEscolhido === pergunta.correta) {
      acertos++;
    }

    explanationBox.textContent = pergunta.explicacao;
    explanationBox.hidden = false;
    nextBtn.hidden = false;
  }

  function proximaPergunta() {
    indiceAtual++;
    if (indiceAtual < perguntasRodada.length) {
      mostrarPergunta();
    } else {
      mostrarResultado();
    }
  }

  function mostrarResultado() {
    var total = perguntasRodada.length;
    var percentual = (acertos / total) * 100;
    var mensagem;

    if (percentual === 100) {
      mensagem = 'Perfeito! Você conhece muito bem a história de Mary Jackson.';
    } else if (percentual >= 70) {
      mensagem = 'Muito bom! Você conhece bem a trajetória dela.';
    } else if (percentual >= 40) {
      mensagem = 'Legal! Vale revisar a linha do tempo para fixar mais detalhes.';
    } else {
      mensagem = 'Vale a pena revisitar a biografia e a linha do tempo acima.';
    }

    progressFill.style.width = '100%';
    resultTitle.textContent = acertos + ' de ' + total + ' corretas';
    resultText.textContent = mensagem;
    ringLabel.textContent = Math.round(percentual) + '%';
    ringFill.style.strokeDasharray = RING_PERIMETRO;
    ringFill.style.strokeDashoffset = RING_PERIMETRO; // reseta antes de animar
    mostrarPainel(quizResult);

    // pequena espera para o navegador aplicar o reset antes de animar
    requestAnimationFrame(function () {
      requestAnimationFrame(function () {
        ringFill.style.strokeDashoffset = RING_PERIMETRO * (1 - percentual / 100);
      });
    });
  }

  quizStartBtn.addEventListener('click', iniciarQuiz);
  nextBtn.addEventListener('click', proximaPergunta);
  retryBtn.addEventListener('click', iniciarQuiz);

});