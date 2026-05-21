import { useState } from 'react';
import { motion } from 'framer-motion';
import {
  Activity,
  ArrowRight,
  Box,
  Cloud,
  Cpu,
  Github,
  Heart,
  Leaf,
  Lightbulb,
  Mic,
  Network,
  Radio,
  Send,
  Sparkles,
  Users,
  Waves,
  Wifi,
  Zap,
} from 'lucide-react';
import NetworkBackground from './components/NetworkBackground.jsx';

const fadeUp = {
  hidden: { opacity: 0, y: 28 },
  visible: { opacity: 1, y: 0 },
};

const stagger = {
  hidden: {},
  visible: {
    transition: {
      staggerChildren: 0.08,
    },
  },
};

const pillars = [
  'Digital fabrication',
  'Embedded electronics',
  'Sensor networks',
  'Community participation',
  'Real-time visualization',
  'Open-source collaboration',
];

const spiralSteps = [
  {
    number: '1',
    title: 'Module development',
    subtitle: 'For the event',
    result: 'Modular, scalable, and ready to deploy.',
    text: 'Fab leaf modules are designed and fabricated with light, sensing, and structural connection in mind.',
  },
  {
    number: '2',
    title: 'Tree installation',
    subtitle: 'Boston / FAB26 MIT',
    result: 'A living installation that visualizes collective behavior.',
    text: 'Modules are assembled into a shared structure where local interaction becomes swarm-like light behavior.',
  },
  {
    number: '3',
    title: 'Modules travel',
    subtitle: 'The project lives on',
    result: 'A distributed network that continues to evolve.',
    text: 'Participants take modules back to their countries, keeping Fab Vine active across the global Fab Lab community.',
  },
];

const technologies = [
  ['Seeed XIAO ESP32-C6', Cpu],
  ['MQTT + WiFi', Wifi],
  ['WS2812B LEDs', Lightbulb],
  ['Audio + motion sensors', Mic],
  ['I2C communication', Radio],
  ['Fab.Cloud swarm engine', Cloud],
  ['Distributed nodes', Network],
  ['Digital fabrication systems', Box],
];

const audiences = ['Fab Labs', 'Makers', 'Designers', 'Engineers', 'Artists', 'Students', 'Researchers'];

function SectionLabel({ children }) {
  return (
    <div className="mb-5 inline-flex items-center gap-2 rounded-full border border-white/10 bg-white/[0.04] px-3 py-1 font-mono text-xs uppercase tracking-[0.24em] text-vine-cyan">
      <Sparkles className="h-3.5 w-3.5" />
      {children}
    </div>
  );
}

function GlowCard({ children, className = '' }) {
  return (
    <motion.div
      variants={fadeUp}
      className={`group relative overflow-hidden rounded-lg border border-white/10 bg-white/[0.045] p-6 shadow-glow backdrop-blur ${className}`}
    >
      <div className="absolute inset-x-0 top-0 h-px bg-gradient-to-r from-transparent via-vine-green/80 to-transparent opacity-70" />
      <div className="absolute -right-10 -top-10 h-28 w-28 rounded-full bg-vine-cyan/10 blur-2xl transition duration-500 group-hover:bg-vine-green/20" />
      {children}
    </motion.div>
  );
}

function Hero() {
  return (
    <section className="relative flex min-h-screen items-center overflow-hidden px-5 pt-24 sm:px-8 lg:px-12">
      <NetworkBackground />
      <div className="absolute inset-0 bg-[radial-gradient(circle_at_50%_35%,rgba(88,255,176,0.14),transparent_34%),radial-gradient(circle_at_78%_62%,rgba(88,246,255,0.11),transparent_28%)]" />
      <div className="absolute inset-0 bg-grid bg-[size:48px_48px] opacity-25 [mask-image:linear-gradient(to_bottom,black,transparent_78%)]" />

      <motion.div
        initial="hidden"
        animate="visible"
        variants={stagger}
        className="relative z-10 mx-auto grid w-full max-w-7xl items-center gap-12 lg:grid-cols-[1.02fr_0.98fr]"
      >
        <div>
          <motion.div variants={fadeUp} className="mb-6 flex flex-wrap items-center gap-3">
            <span className="rounded-full border border-vine-green/30 bg-vine-green/10 px-3 py-1 font-mono text-xs uppercase tracking-[0.22em] text-vine-mint">
              Tree of Connection / FAB26 MIT
            </span>
            <span className="rounded-full border border-vine-cyan/25 bg-vine-cyan/10 px-3 py-1 font-mono text-xs uppercase tracking-[0.22em] text-vine-cyan">
              Living prototype
            </span>
          </motion.div>

          <motion.h1
            variants={fadeUp}
            className="title-glow font-display text-6xl font-semibold leading-none text-white sm:text-7xl md:text-8xl lg:text-9xl"
          >
            Fab Vine
          </motion.h1>

          <motion.p variants={fadeUp} className="mt-6 max-w-2xl text-2xl leading-tight text-vine-mint md:text-3xl">
            A living network of fabrication, data, and human connection.
          </motion.p>

          <motion.p variants={fadeUp} className="mt-6 max-w-2xl text-base leading-8 text-white/68 md:text-lg">
            Fab Vine turns modular fabricated leaves into a distributed sensing network. Local sound, motion, web
            interaction, and remote Fab Lab activity are processed through a swarm engine and translated into light,
            behavior, and collective expression.
          </motion.p>

          <motion.div variants={fadeUp} className="mt-9 flex flex-col gap-3 sm:flex-row">
            <a href="#join" className="inline-flex items-center justify-center gap-2 rounded-full bg-vine-green px-6 py-3 font-semibold text-vine-black shadow-glow transition hover:-translate-y-0.5 hover:bg-vine-mint">
              Join the Community
              <Send className="h-4 w-4" />
            </a>
            <a href="#methodology" className="inline-flex items-center justify-center gap-2 rounded-full border border-white/15 bg-white/[0.04] px-6 py-3 font-semibold text-white backdrop-blur transition hover:-translate-y-0.5 hover:border-vine-cyan/60 hover:text-vine-cyan">
              Explore the Spiral
              <Network className="h-4 w-4" />
            </a>
          </motion.div>
        </div>

        <motion.div variants={fadeUp} className="relative mx-auto aspect-square w-full max-w-[540px]">
          <div className="absolute inset-4 rounded-[2rem] border border-vine-green/20 bg-vine-green/[0.03] shadow-glow" />
          <div className="absolute inset-12 rounded-full border border-vine-cyan/25" />
          <div className="vine-orbit absolute inset-0 rounded-full border border-dashed border-white/10" />
          <div className="absolute left-1/2 top-1/2 grid h-32 w-32 -translate-x-1/2 -translate-y-1/2 place-items-center rounded-[1.7rem] border border-vine-green/40 bg-vine-green/10 shadow-glow">
            <Leaf className="h-12 w-12 text-vine-green" />
          </div>
          {Array.from({ length: 12 }).map((_, index) => {
            const angle = (index / 12) * Math.PI * 2;
            const x = 50 + Math.cos(angle) * 41;
            const y = 50 + Math.sin(angle) * 41;
            return (
              <span
                key={index}
                className="absolute h-3 w-3 rounded-full bg-vine-cyan shadow-cyanGlow"
                style={{ left: `${x}%`, top: `${y}%`, animationDelay: `${index * 0.2}s` }}
              />
            );
          })}
          <div className="absolute bottom-8 left-8 right-8 rounded-lg border border-white/10 bg-black/35 p-4 font-mono text-xs text-white/70 backdrop-blur">
            <div className="mb-2 text-vine-green">fab.cloud/swarm_engine</div>
            <div className="grid grid-cols-3 gap-3">
              <span>sound</span>
              <span>motion</span>
              <span>light_rules</span>
            </div>
          </div>
        </motion.div>
      </motion.div>
    </section>
  );
}

function About() {
  return (
    <section id="about" className="px-5 py-24 sm:px-8 lg:px-12">
      <motion.div initial="hidden" whileInView="visible" viewport={{ once: true, amount: 0.2 }} variants={stagger} className="mx-auto max-w-7xl">
        <SectionLabel>About the project</SectionLabel>
        <div className="grid gap-10 lg:grid-cols-[0.95fr_1.05fr]">
          <motion.div variants={fadeUp}>
            <h2 className="font-display text-4xl font-semibold leading-tight text-white md:text-6xl">
              A fabricated organism made of modules, people, and signals.
            </h2>
            <p className="mt-6 text-lg leading-8 text-white/66">
              Fab Vine combines modular fabrication, embedded electronics, biomaterial-inspired forms, and community
              participation. Each node acts like a digital leaf connected to a larger living organism.
            </p>
            <p className="mt-4 font-mono text-sm text-vine-cyan/80">
              Red viva de fabricacion, datos y conexion humana.
            </p>
          </motion.div>
          <motion.div variants={stagger} className="grid gap-4 sm:grid-cols-2">
            {pillars.map((item) => (
              <GlowCard key={item}>
                <Leaf className="mb-5 h-7 w-7 text-vine-green" />
                <h3 className="font-display text-xl font-semibold text-white">{item}</h3>
              </GlowCard>
            ))}
          </motion.div>
        </div>
      </motion.div>
    </section>
  );
}

function SpiralMethodology() {
  return (
    <section id="methodology" className="relative overflow-hidden px-5 py-24 sm:px-8 lg:px-12">
      <div className="absolute inset-0 bg-[radial-gradient(circle_at_25%_20%,rgba(88,255,176,0.1),transparent_32%),radial-gradient(circle_at_80%_60%,rgba(120,80,255,0.13),transparent_34%)]" />
      <motion.div initial="hidden" whileInView="visible" viewport={{ once: true, amount: 0.2 }} variants={stagger} className="relative mx-auto max-w-7xl">
        <SectionLabel>Spiral methodology</SectionLabel>
        <div className="grid gap-10 lg:grid-cols-[0.78fr_1.22fr]">
          <motion.div variants={fadeUp}>
            <h2 className="font-display text-4xl font-semibold leading-tight text-white md:text-6xl">
              The cycle continues beyond the event.
            </h2>
            <p className="mt-6 text-lg leading-8 text-white/66">
              Inspired by the MIT presentation, Fab Vine evolves through a spiral process: fabricate modules, activate
              the tree at FAB26, then let modules travel back into the world as distributed Fab Lab nodes.
            </p>
            <div className="mt-8 rounded-lg border border-vine-green/20 bg-vine-green/[0.06] p-5 font-mono text-sm text-vine-mint">
              Sense locally <ArrowRight className="mx-2 inline h-4 w-4" /> share globally
              <ArrowRight className="mx-2 inline h-4 w-4" /> act collectively
              <ArrowRight className="mx-2 inline h-4 w-4" /> keep the community alive
            </div>
          </motion.div>

          <motion.div variants={stagger} className="grid gap-4 lg:grid-cols-3">
            {spiralSteps.map((step) => (
              <GlowCard key={step.number} className="min-h-[360px]">
                <div className="flex h-16 w-16 items-center justify-center rounded-full border border-white/15 bg-white/10 font-display text-3xl font-semibold text-vine-green">
                  {step.number}
                </div>
                <p className="mt-8 font-mono text-xs uppercase tracking-[0.2em] text-vine-cyan">{step.subtitle}</p>
                <h3 className="mt-3 font-display text-2xl font-semibold text-white">{step.title}</h3>
                <p className="mt-4 text-sm leading-7 text-white/60">{step.text}</p>
                <div className="mt-6 rounded-lg border border-white/10 bg-black/25 p-4">
                  <div className="font-mono text-xs uppercase tracking-[0.18em] text-vine-green">Result</div>
                  <p className="mt-2 text-sm leading-6 text-white/72">{step.result}</p>
                </div>
              </GlowCard>
            ))}
          </motion.div>
        </div>
      </motion.div>
    </section>
  );
}

function Fab26() {
  return (
    <section id="fab26" className="relative overflow-hidden px-5 py-24 sm:px-8 lg:px-12">
      <div className="absolute inset-0 bg-[linear-gradient(120deg,rgba(88,255,176,0.08),transparent_45%,rgba(88,246,255,0.08))]" />
      <motion.div initial="hidden" whileInView="visible" viewport={{ once: true, amount: 0.2 }} variants={stagger} className="relative mx-auto max-w-7xl">
        <SectionLabel>FAB26 Cambridge/MIT</SectionLabel>
        <div className="grid gap-8 lg:grid-cols-[0.8fr_1.2fr]">
          <GlowCard className="flex min-h-72 flex-col justify-between">
            <div className="flex h-24 w-24 items-center justify-center rounded-lg border border-vine-cyan/25 bg-vine-cyan/10 font-mono text-sm uppercase tracking-[0.2em] text-vine-cyan">
              FAB26
            </div>
            <div>
              <h2 className="font-display text-4xl font-semibold text-white">Cambridge / MIT 2026</h2>
              <p className="mt-5 leading-7 text-white/65">
                Fab Vine will be assembled as a collective Tree of Connection during FAB26, turning physical presence
                and digital inputs into live behavior.
              </p>
            </div>
          </GlowCard>
          <motion.div variants={stagger} className="grid gap-4 md:grid-cols-3">
            {spiralSteps.map((step) => (
              <GlowCard key={step.title}>
                <div className="font-mono text-sm text-vine-green">0{step.number}</div>
                <h3 className="mt-8 font-display text-2xl font-semibold text-white">{step.title}</h3>
                <p className="mt-4 text-sm leading-7 text-white/60">{step.result}</p>
              </GlowCard>
            ))}
          </motion.div>
        </div>
        <motion.p variants={fadeUp} className="mt-10 max-w-4xl text-2xl leading-snug text-white/78">
          "Fab Vine is an open invitation to the global Fab Lab community to collaboratively build the next generation of
          connected creative ecosystems."
        </motion.p>
      </motion.div>
    </section>
  );
}

function SignalSimulator() {
  const [localActivity, setLocalActivity] = useState(62);
  const [webInput, setWebInput] = useState(38);
  const [remoteLabs, setRemoteLabs] = useState(74);
  const swarm = Math.round(localActivity * 0.45 + webInput * 0.2 + remoteLabs * 0.35);
  const colorShift = Math.round((webInput + remoteLabs) / 2);

  return (
    <GlowCard className="lg:col-span-2">
      <div className="grid gap-8 lg:grid-cols-[0.95fr_1.05fr]">
        <div>
          <div className="font-mono text-xs uppercase tracking-[0.2em] text-vine-green">Interactive behavior console</div>
          <h3 className="mt-4 font-display text-3xl font-semibold text-white">Test the swarm engine logic.</h3>
          <p className="mt-4 leading-7 text-white/62">
            Move the signal inputs to see how local interaction, website participation, and remote Fab Labs could shape
            the installation behavior.
          </p>
          {[
            ['Local sound + motion', localActivity, setLocalActivity],
            ['Website interaction', webInput, setWebInput],
            ['Remote Fab Lab activity', remoteLabs, setRemoteLabs],
          ].map(([label, value, setter]) => (
            <label key={label} className="mt-5 block">
              <span className="mb-2 flex justify-between font-mono text-xs uppercase tracking-[0.16em] text-white/45">
                {label}
                <span className="text-vine-cyan">{value}%</span>
              </span>
              <input
                type="range"
                min="0"
                max="100"
                value={value}
                onChange={(event) => setter(Number(event.target.value))}
                className="w-full accent-vine-green"
              />
            </label>
          ))}
        </div>
        <div className="relative min-h-80 overflow-hidden rounded-lg border border-white/10 bg-black/25 p-6">
          <div
            className="absolute inset-8 rounded-full blur-3xl transition duration-500"
            style={{ background: `rgba(${88 + colorShift}, 255, ${176 + Math.round(colorShift / 2)}, ${0.08 + swarm / 700})` }}
          />
          <div className="relative grid h-full place-items-center">
            <div className="relative h-56 w-56">
              <div className="absolute inset-0 rounded-full border border-vine-green/20" />
              <div className="absolute inset-8 rounded-full border border-vine-cyan/30" />
              <div
                className="absolute left-1/2 top-1/2 grid -translate-x-1/2 -translate-y-1/2 place-items-center rounded-full border border-vine-green/40 bg-vine-green/10 text-center shadow-glow transition-all duration-500"
                style={{ width: `${96 + swarm}px`, height: `${96 + swarm}px` }}
              >
                <span className="font-display text-4xl font-semibold text-white">{swarm}</span>
                <span className="font-mono text-xs uppercase tracking-[0.16em] text-vine-green">swarm</span>
              </div>
            </div>
            <div className="mt-4 grid grid-cols-3 gap-3 font-mono text-xs text-white/56">
              <span>capture</span>
              <span>normalize</span>
              <span>light rules</span>
            </div>
          </div>
        </div>
      </div>
    </GlowCard>
  );
}

function Technology() {
  return (
    <section id="technology" className="px-5 py-24 sm:px-8 lg:px-12">
      <motion.div initial="hidden" whileInView="visible" viewport={{ once: true, amount: 0.2 }} variants={stagger} className="mx-auto max-w-7xl">
        <SectionLabel>Technology and interaction</SectionLabel>
        <div className="mb-10 flex flex-col justify-between gap-6 md:flex-row md:items-end">
          <h2 className="max-w-3xl font-display text-4xl font-semibold leading-tight text-white md:text-6xl">
            From leaf node inputs to color-coded swarm behavior.
          </h2>
          <div className="rounded-lg border border-white/10 bg-white/[0.04] px-4 py-3 font-mono text-xs text-white/60">
            engine: <span className="text-vine-green">capture / normalize / calculate / generate</span>
          </div>
        </div>

        <motion.div variants={stagger} className="grid gap-4 sm:grid-cols-2 lg:grid-cols-4">
          {technologies.map(([name, Icon]) => (
            <GlowCard key={name} className="min-h-44">
              <Icon className="h-7 w-7 text-vine-cyan" />
              <h3 className="mt-10 font-display text-xl font-semibold text-white">{name}</h3>
            </GlowCard>
          ))}
          <SignalSimulator />
        </motion.div>
      </motion.div>
    </section>
  );
}

function Join() {
  return (
    <section id="join" className="px-5 py-24 sm:px-8 lg:px-12">
      <motion.div initial="hidden" whileInView="visible" viewport={{ once: true, amount: 0.15 }} variants={stagger} className="mx-auto grid max-w-7xl gap-10 lg:grid-cols-[0.9fr_1.1fr]">
        <motion.div variants={fadeUp}>
          <SectionLabel>Join the community</SectionLabel>
          <h2 className="font-display text-4xl font-semibold leading-tight text-white md:text-6xl">
            Build a module. Host a node. Keep the network alive.
          </h2>
          <p className="mt-6 text-lg leading-8 text-white/66">
            Contributors will help shape the installation, hardware, software, interaction systems, module library, and
            post-event global experience.
          </p>
          <div className="mt-8 flex flex-wrap gap-2">
            {audiences.map((audience) => (
              <span key={audience} className="rounded-full border border-vine-green/20 bg-vine-green/[0.08] px-3 py-1 text-sm text-vine-mint">
                {audience}
              </span>
            ))}
          </div>
        </motion.div>

        <GlowCard>
          <form className="grid gap-4 sm:grid-cols-2">
            {['Name', 'Country', 'Fab Lab / Organization', 'Skills', 'GitHub username', 'Email'].map((label) => (
              <label key={label} className="block">
                <span className="mb-2 block font-mono text-xs uppercase tracking-[0.18em] text-white/45">{label}</span>
                <input
                  type={label === 'Email' ? 'email' : 'text'}
                  placeholder={label}
                  className="w-full rounded-lg border border-white/10 bg-black/30 px-4 py-3 text-white outline-none transition placeholder:text-white/25 focus:border-vine-green/70 focus:shadow-glow"
                />
              </label>
            ))}
            <button type="button" className="mt-2 inline-flex items-center justify-center gap-2 rounded-full bg-vine-cyan px-6 py-3 font-semibold text-vine-black transition hover:-translate-y-0.5 hover:bg-white sm:col-span-2">
              Register Interest
              <Users className="h-4 w-4" />
            </button>
          </form>
        </GlowCard>
      </motion.div>
    </section>
  );
}

function Footer() {
  return (
    <footer className="border-t border-white/10 px-5 py-10 sm:px-8 lg:px-12">
      <div className="mx-auto flex max-w-7xl flex-col gap-6 md:flex-row md:items-center md:justify-between">
        <div>
          <div className="font-display text-2xl font-semibold text-white">Fab Vine</div>
          <div className="mt-2 font-mono text-sm text-vine-green">"Grow the network."</div>
        </div>
        <nav className="flex flex-wrap gap-4 text-sm text-white/58">
          <a className="transition hover:text-vine-green" href="https://github.com/FAB-VINE" target="_blank" rel="noreferrer">GitHub</a>
          <a className="transition hover:text-vine-green" href="https://www.instagram.com/" target="_blank" rel="noreferrer">Instagram</a>
          <a className="transition hover:text-vine-green" href="https://fabfoundation.org/" target="_blank" rel="noreferrer">Fab Labs</a>
          <a className="transition hover:text-vine-green" href="#fab26">FAB26</a>
        </nav>
      </div>
    </footer>
  );
}

export default function App() {
  return (
    <main className="min-h-screen overflow-hidden bg-vine-black text-white">
      <Hero />
      <About />
      <SpiralMethodology />
      <Fab26 />
      <Technology />
      <Join />
      <Footer />
    </main>
  );
}
