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
          black: '#071019',
          paper: '#f7f8f4',
          mist: '#eef2f5',
          green: '#0f4f12',
          deepGreen: '#082f0b',
          blue: '#073277',
          cyan: '#075f84',
          purple: '#5a2ea6',
          mint: '#dff4df',
          moss: '#6f8f55',
          copper: '#d89a55',
          bark: '#5f4934',
        },
      },
      boxShadow: {
        glow: '0 18px 50px rgba(30, 111, 22, 0.16)',
        cyanGlow: '0 0 38px rgba(22, 75, 160, 0.18)',
        analog: '0 24px 80px rgba(7, 16, 25, 0.12), inset 0 1px 0 rgba(255, 255, 255, 0.8)',
      },
      backgroundImage: {
        grid: 'linear-gradient(rgba(22, 75, 160, 0.07) 1px, transparent 1px), linear-gradient(90deg, rgba(30, 111, 22, 0.07) 1px, transparent 1px)',
      },
    },
  },
  plugins: [],
};
