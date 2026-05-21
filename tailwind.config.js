/** @type {import('tailwindcss').Config} */
export default {
  content: ['./index.html', './src/**/*.{js,jsx}'],
  theme: {
    extend: {
      fontFamily: {
        display: ['Space Grotesk', 'Inter', 'system-ui', 'sans-serif'],
        sans: ['Inter', 'system-ui', 'sans-serif'],
        mono: ['IBM Plex Mono', 'ui-monospace', 'SFMono-Regular', 'monospace'],
      },
      colors: {
        vine: {
          black: '#030706',
          night: '#07100f',
          green: '#58ffb0',
          cyan: '#58f6ff',
          mint: '#b8ffd9',
        },
      },
      boxShadow: {
        glow: '0 0 40px rgba(88, 255, 176, 0.2)',
        cyanGlow: '0 0 50px rgba(88, 246, 255, 0.2)',
      },
      backgroundImage: {
        grid: 'linear-gradient(rgba(88, 255, 176, 0.08) 1px, transparent 1px), linear-gradient(90deg, rgba(88, 246, 255, 0.07) 1px, transparent 1px)',
      },
    },
  },
  plugins: [],
};
